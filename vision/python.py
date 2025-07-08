'''
CanMV K230视频流推送程序
功能：
1. 自动连接WiFi
2. 连接到远程服务器
3. 捕获摄像头图像并推送到服务器
'''

import network, socket, time, os, sys
from machine import Pin
import time, gc

from media.sensor import * # 导入sensor模块，使用摄像头相关接口
from media.display import * # 导入display模块，使用display相关接口
from media.media import * # 导入media模块，使用media相关接口

# WiFi配置
WIFI_SSID = '300pro'  # WiFi名称
WIFI_PASSWORD = '12345678'  # WiFi密码

# 服务器配置
SERVER_IP = '1.92.66.67'  # 服务器IP地址
SERVER_PORT = 9992        # 服务器端口

# 全局变量
wifi_connected = False
socket_connected = False
client_socket = None

# 初始化摄像头
def init_camera():
    global sensor

    sensor = Sensor()  # 构建摄像头对象
    sensor.reset()     # 复位和初始化摄像头
    sensor.set_framesize(Sensor.HD)  # 设置帧大小HD(1280x720)，默认通道0
    sensor.set_pixformat(Sensor.RGB565)  # 设置输出图像格式，默认通道0

    # 使用IDE缓冲区输出图像,显示尺寸和sensor配置一致
    Display.init(Display.VIRT, sensor.width(), sensor.height())

    MediaManager.init()  # 初始化media资源管理器
    sensor.run()         # 启动sensor

    print('Camera initialized successfully')
    return sensor

# WiFi连接函数
def wifi_connect():
    global wifi_connected

    WIFI_LED = Pin(52, Pin.OUT)  # 初始化WIFI指示灯

    wlan = network.WLAN(network.STA_IF)  # STA模式
    wlan.active(True)                   # 激活接口

    if not wlan.isconnected():
        print('Connecting to WiFi network...')

        for i in range(3):  # 重复连接3次
            # 连接WiFi，连接超过5秒为超时
            wlan.connect(WIFI_SSID, WIFI_PASSWORD)

            # 等待连接成功或超时
            start_time = time.time()
            while not wlan.isconnected() and time.time() - start_time < 5:
                time.sleep_ms(100)

            if wlan.isconnected():  # 连接成功
                break

    if wlan.isconnected():  # 连接成功
        print('WiFi connected successfully')

        # LED蓝灯点亮
        WIFI_LED.value(1)

        # 等待获取IP地址
        while wlan.ifconfig()[0] == '0.0.0.0':
            time.sleep_ms(100)

        # 打印网络信息
        print('Network information:', wlan.ifconfig())
        wifi_connected = True
        return wlan

    else:  # 连接失败
        print('WiFi connection failed')

        # LED闪烁3次提示
        for i in range(3):
            WIFI_LED.value(1)
            time.sleep_ms(300)
            WIFI_LED.value(0)
            time.sleep_ms(300)

        wifi_connected = False
        return None

# 连接到视频服务器
def connect_to_server():
    global client_socket, socket_connected

    try:
        # 创建socket对象
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.settimeout(5)  # 设置连接超时为5秒

        # 获取服务器地址格式
        addr = socket.getaddrinfo(SERVER_IP, SERVER_PORT)[0][-1]
        print(f"Connecting to server: {addr}")

        # 连接服务器
        client_socket.connect(addr)
        print("Connected to server successfully")

        # 发送连接成功消息
        client_socket.send(b'K230 Camera Online')

        socket_connected = True
        return True

    except Exception as e:
        print(f"Server connection error: {e}")
        if client_socket:
            client_socket.close()
        socket_connected = False
        return False

# 尝试重新连接服务器
def try_reconnect_server():
    global client_socket, socket_connected

    # 如果socket存在，先关闭
    if client_socket:
        try:
            client_socket.close()
        except:
            pass

    socket_connected = False
    print("Trying to reconnect to server...")

    # 尝试重新连接
    for i in range(3):  # 重试3次
        if connect_to_server():
            return True
        time.sleep(2)

    print("Reconnection failed after 3 attempts")
    return False

# 将图像转换为字节流
def image_to_bytes(img):
    try:
        # 将图像转换为JPEG格式的字节流
        img_bytes = img.compress(quality=70)
        return img_bytes
    except Exception as e:
        print(f"Image conversion error: {e}")
        return None

# 发送图像到服务器
def send_image(img_bytes):
    global client_socket, socket_connected

    if not socket_connected or client_socket is None:
        return False

    try:
        if img_bytes:
            # 准备帧大小的头部信息
            frame_size = len(img_bytes)
            header = frame_size.to_bytes(4, 'big')

            # 尝试发送头部和图像数据
            client_socket.send(header + img_bytes)
            return True
    except OSError as e:
        # 特别处理特定类型的错误
        if hasattr(e, 'errno') and e.errno in [103, 104, 32]:  # 连接重置或中断
            print(f"连接已断开: {e}")
        else:
            print(f"发送图像时出错: {e}")
        socket_connected = False
        return False
    except Exception as e:
        print(f"发送图像未知错误: {e}")
        socket_connected = False
        return False

# 主程序
def main():
    global wifi_connected, socket_connected, client_socket

    # 初始化摄像头
    camera = init_camera()
    if not camera:
        print("Camera initialization failed")
        return

    # 创建FPS计时器
    clock = time.clock()

    # 连接状态变量
    last_reconnect_time = 0
    last_frame_time = 0
    frame_interval = 0.04  # 控制帧率约25FPS
    reconnect_interval = 10  # 重连间隔(秒)

    # 控制信息显示的频率
    info_display_counter = 0

    # 添加性能监控变量
    frames_sent = 0
    frames_captured = 0
    start_monitor_time = time.time()
    last_stats_time = time.time()

    while True:
        try:
            current_time = time.time()

            # WiFi连接检查
            if not wifi_connected:
                wlan = wifi_connect()
                if not wlan:
                    time.sleep(5)  # 连接失败，等待5秒后重试
                    continue

            # 服务器连接检查
            if wifi_connected and not socket_connected:
                if current_time - last_reconnect_time > reconnect_interval:
                    connect_to_server()
                    last_reconnect_time = current_time

            # 控制帧率
            if current_time - last_frame_time < frame_interval:
                time.sleep_ms(5)
                continue

            last_frame_time = current_time
            clock.tick()  # 更新FPS计时器

            # 捕获图像
            img = sensor.snapshot()  # 拍摄一张图
            frames_captured += 1

            # 在图像上添加状态信息
            status = "Connected" if socket_connected else "Disconnected"
            wifi_status = "WiFi: Connected" if wifi_connected else "WiFi: Disconnected"

            # 每10帧更新一次状态信息（减少文本叠加对性能的影响）
            info_display_counter += 1
            if info_display_counter % 10 == 0:
                fps_text = f"FPS: {clock.fps():.1f}"
                img.draw_string(10, 10, fps_text, color=(0, 255, 0), scale=2)
                img.draw_string(10, 40, status, color=(0, 255, 0), scale=2)
                img.draw_string(10, 70, wifi_status, color=(0, 255, 0), scale=2)

                # 显示发送统计
                if frames_sent > 0:
                    sent_text = f"Sent: {frames_sent}"
                    img.draw_string(10, 100, sent_text, color=(0, 255, 0), scale=2)

                info_display_counter = 0

            # 显示图像到本地Display
            Display.show_image(img)

            # 如果连接到了服务器，发送图像
            if socket_connected:
                img_bytes = image_to_bytes(img)
                send_success = send_image(img_bytes)

                if send_success:
                    frames_sent += 1

                    # 每30秒显示一次性能统计
                    if current_time - last_stats_time > 30:
                        elapsed = current_time - start_monitor_time
                        if elapsed > 0:
                            avg_send_fps = frames_sent / elapsed
                            avg_capture_fps = frames_captured / elapsed
                            print("性能统计:")
                            print(f"- 运行时间: {elapsed:.1f}秒")
                            print(f"- 平均捕获帧率: {avg_capture_fps:.1f}FPS")
                            print(f"- 平均发送帧率: {avg_send_fps:.1f}FPS")
                            print(f"- 已捕获: {frames_captured}帧")
                            print(f"- 已发送: {frames_sent}帧")
                            print(f"- 发送率: {(frames_sent/frames_captured*100) if frames_captured > 0 else 0:.1f}%")
                            last_stats_time = current_time

                # 如果发送失败，尝试重新连接
                if not send_success and current_time - last_reconnect_time > reconnect_interval:
                    try_reconnect_server()
                    last_reconnect_time = current_time

            # 主动垃圾回收，防止内存泄漏
            if info_display_counter % 50 == 0:
                gc.collect()

        except KeyboardInterrupt:
            print("Program interrupted by user")
            break
        except Exception as e:
            print(f"Error in main loop: {e}")
            time.sleep(1)  # 出错后短暂延时，防止过快循环

    # 程序结束，清理资源
    if client_socket:
        try:
            client_socket.close()
        except:
            pass

    # 显示最终统计信息
    elapsed = time.time() - start_monitor_time
    if elapsed > 0 and frames_captured > 0:
        print("\n最终统计信息:")
        print(f"- 总运行时间: {elapsed:.1f}秒")
        print(f"- 总捕获帧数: {frames_captured}帧")
        print(f"- 总发送帧数: {frames_sent}帧")
        print(f"- 平均捕获帧率: {frames_captured/elapsed:.1f}FPS")
        print(f"- 平均发送帧率: {frames_sent/elapsed:.1f}FPS")
        print(f"- 总体发送成功率: {(frames_sent/frames_captured*100):.1f}%")

    print("Program terminated")

# 程序入口点
if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"Fatal error: {e}")
