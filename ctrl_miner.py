#miners addr
#192.168.1.158 = vega56 = 2C:F0:5D:5A:34:AC
import time, socket, logging
from wakeonlan import send_magic_packet

mac = "2C:F0:5D:5A:34:AC"

logging.basicConfig(filename='stats/log_ctrl_miner', level=logging.DEBUG, format="%(asctime)s | %(message)s", datefmt="%d-%m-%Y %H:%M:%S")

def read_stat(s):
    f = open("stats/"+s, "r")
    return f.read()

def message(addr, mess):
	try:
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.connect((addr, 2000))
		s.sendall(mess.encode('utf-8'))
		re = (s.recv(1024)).decode('utf-8')
		s.close()
		return re
	except:
		return None

try:
	miner_stat = message('192.168.1.158', 'ping')
	if miner_stat == 'pong':
		logging.info("192.168.1.158 => already wake")
	else: 
		logging.info("192.168.1.158 => already sleep")
except:
	miner_stat = None
	logging.info("192.168.1.158 => already sleep")

while True:
	try:
		v_bat = float(read_stat('v_bat'))
		if miner_stat == "pong" and v_bat <= 24.0:
			message("192.168.1.158", "sleep")
			miner_stat = None
			logging.info("192.168.1.158 => sleep")
		elif v_bat >= 27:
			#wake("2C:F0:5D:5A:34:AC")
			send_magic_packet(mac)
			miner_stat = "pong"
			logging.info("192.168.1.158 => wake")
		time.sleep(1)
	except:
		unixTime = time.time()
		print(str(unixTime)+" error! just try again.")