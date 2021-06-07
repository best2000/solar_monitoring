import logging, time, requests

logging.basicConfig(filename='stats/log', level=logging.INFO, format="%(asctime)s | %(message)s", datefmt="%d-%m-%Y %H:%M:%S")
logging.getLogger("requests").setLevel(logging.DEBUG)

def write_stat(data, file):
	f = open("stats/"+file, "w")
	f.write(str(data))
	f.close()

start = time.time()
while True:
	try:
		r = requests.get('http://192.168.1.162')
		s = r.json()
		v_pv = float(s['v_pv'])
		i_pv = float(s['i_pv'])
		v_bat = float(s['v_bat'])
		i_load = float(s['i_load'])
		p_pv = int(v_pv*i_pv)
		p_out = int(v_bat*i_load)
		write_stat(v_pv, 'v_pv')
		write_stat(i_pv, 'i_pv')
		write_stat(v_bat, 'v_bat')
		write_stat(i_load, 'i_load')
		if time.time()-start >= 60:
			logging.info(format(v_pv, '.2f')+" | "+format(i_pv, '.2f')+" | "+str(p_pv)+" | "+format(v_bat, '.2f')+" | "+format(i_load, '.2f')+" | "+str(p_out))
			start = time.time()
		time.sleep(1)
	except:
		unixTime = time.time()
		print(str(unixTime)+" error! just try again.")
