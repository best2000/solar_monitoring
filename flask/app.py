from flask import Flask, render_template, jsonify, request, url_for
from datetime import datetime
import os

def read_stat(s):
    f = open("/home/pi/solar_monitoring/stats/"+s, "r")
    #f = open("C:/repo/solar_automa/stats/"+s, "r")
    return f.read()

def read_log(l):
    x = []
    y = []
    
    f = open("/home/pi/solar_monitoring/stats/log.txt", "r")
    #f = open("C:/repo/solar_automa/stats/log.txt", "r")
    lines = f.read().splitlines()
    f.close()
        
    split_lines = []
    for line in lines:
        split_lines.append(line.split("|"))
    
    #list of splited => [[time1, val1], [time2, val2], [time3, val3]]

    date_time=[]
    v_pv=[]
    i_pv=[]
    p_pv=[]
    v_bat=[]
    i_load=[]
    p_load=[]

    for line in split_lines:
        date_time.append(line[0])
        v_pv.append(line[1])
        i_pv.append(line[2])
        p_pv.append(line[3])
        v_bat.append(line[4])
        i_load.append(line[5])
        p_load.append(line[6])

    #extract splitted lis to seperate lis => x=[time1, time2, time3]

    x_date_time = []
    y_v_pv = []
    y_i_pv = []
    y_p_pv = []
    y_v_bat = []
    y_i_load = []
    y_p_load = []
    for i in range(l):
        try:
            if l <= 1440:
                x_date_time.append((date_time.pop())[11:16])
            else:
                x_date_time.append(date_time.pop())
                
            y_v_pv.append(v_pv.pop())
            y_i_pv.append(i_pv.pop())
            y_p_pv.append(p_pv.pop())
            y_v_bat.append(v_bat.pop())
            y_i_load.append(i_load.pop())
            y_p_load.append(p_load.pop())
        except:
            break

    x_date_time.reverse()
    y_v_pv.reverse()
    y_i_pv.reverse()
    y_p_pv.reverse()
    y_v_bat.reverse()
    y_i_load.reverse()
    y_p_load.reverse()

    return {
        'date_time':x_date_time,
        'v_pv':y_v_pv,
        'i_pv':y_i_pv,
        'p_pv':y_p_pv,
        'v_bat':y_v_bat,
        'i_load':y_i_load,
        'p_load':y_p_load }

pics = os.path.join('static', 'pics')
app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = pics

@app.route('/')
def index():
    return render_template('index.html')
  
@app.route('/stats', methods=['GET'])
def get_stats():
    dt = datetime.now()
    date = dt.strftime("%d/%m/%Y")
    time = dt.strftime("%H:%M:%S")
    v_pv = format(float(read_stat("v_pv")), '.2f')
    i_pv = format(float(read_stat("i_pv")), '.2f')
    v_bat = format(float(read_stat("v_bat")), '.2f')
    i_load = format(float(read_stat("i_load")), '.2f')
    return jsonify(date=date, 
                    time=time,
                    v_pv=v_pv,
                    i_pv=i_pv,
                    v_bat=v_bat,
                    i_load=i_load)

@app.route('/logs', methods=['GET'])
def get_logs():
    if request.args:
        l = int(request.args["l"])
        logs = read_log(l)
        return jsonify(date_time=logs['date_time'],
                        v_pv=logs['v_pv'],
                        i_pv=logs['i_pv'],
                        p_pv=logs['p_pv'],
                        v_bat=logs['v_bat'],
                        i_load=logs['i_load'],
                        p_load=logs['p_load'])

@app.route('/info', methods=['GET'])
def info():
    im = os.path.join(app.config['UPLOAD_FOLDER'], '190193463_185863083443360_6031042187570792335_n.jpg')
    im2 = os.path.join(app.config['UPLOAD_FOLDER'], '190591652_131566325640132_3981017436805099639_n.jpg')
    im3 = os.path.join(app.config['UPLOAD_FOLDER'], '190658933_333077144855247_737699591907039461_n.jpg')
    im4 = os.path.join(app.config['UPLOAD_FOLDER'], '193323532_1186615521817270_6973439960209070890_n.jpg')
    im5 = os.path.join(app.config['UPLOAD_FOLDER'], '192446184_548249336517209_1545093652385918260_n.jpg')
    im6 = os.path.join(app.config['UPLOAD_FOLDER'], '194845412_176514641145888_8843008020085624582_n.jpg')
    im7 = os.path.join(app.config['UPLOAD_FOLDER'], '193376441_4111715182237869_757037507244538712_n.jpg')
    
    return render_template("info.html",im = im,im2 = im2,im3 = im3,im4 = im4,im5 = im5,im6 = im6,im7=im7)


if __name__ == "__main__":
    app.run(debug=True)  
