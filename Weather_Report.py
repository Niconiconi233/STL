import requests,json,smtplib,datetime,time
from email.mime.text import MIMEText
from email.header import Header

city = 'city.json'
url = 'http://t.weather.sojson.com/api/weather/city/'
mail_host = 'smtp.qq.com'
mail_user = '1660994874@qq.com'
mail_pass = 'clorqigyqmjycjbg'

sender = 'weather_report'
receivers = ['1660994874@qq.com']

#get city code
def get_city_code(city_name):
    with open(city, 'rb')as f:
        city_codes = json.load(f)
    for city_code in city_codes:
        try:
            if city_code['city_name'] == city_name:
                if(city_code['city_code'] != None):
                    get_weather_json(city_code['city_code'])
        except Exception:
            print('error')

def get_weather_json(city_code):
    w_json = requests.get(url+city_code)
    if w_json.status_code == 200:
        weather = json.loads(w_json.text)
        city_info = weather['cityInfo']
        date = weather['date']
        today_weather = weather['data']
        data = {
               'City':city_info['city'],
               'UpdateTime':city_info['updateTime'],
               'date':date,
               'shidu':today_weather['shidu'],
               'pm2.5':today_weather['pm25'],
               'quality':today_weather['quality'],
               'wendu':today_weather['wendu'],
            }
        send_mail(data)

def send_mail(data):
    mail_msg = """<center>
        <h1>Weather Report</h1>
        <p>城市 :"""+data['City']+"""</p>
        <p>更新时间 :"""+data['UpdateTime']+"""</p>
        <p>日期 :"""+data['date']+"""</p>
        <p>湿度 :"""+data['shidu']+"""</p>
        <p>Pm2.5 :"""+str(data['pm2.5'])+""" ug/m^3</p>
        <p>空气质量 :"""+data['quality']+"""</p>
        <p>温度 :"""+str(data['wendu'])+""" ℃</p></center>
    """
    print('msg')
    message = MIMEText(mail_msg, 'html', 'utf-8')
    message['From'] = Header('weather robot', 'utf-8')
    message['To'] = Header('master', 'utf-8')
    subject = 'auto weather robot'
    message['Subject'] = Header(subject, 'utf-8')

    try:
        server = smtplib.SMTP_SSL(mail_host, 465)
        server.login(mail_user, mail_pass)
        server.sendmail(mail_user, receivers, message.as_string())
        server.quit()
        print('success')
    except Excetion:
        print('error')


def main():
    print('weather robot start!')
    times = 1
    while True:
        da = datetime.datetime.now()
        if da.hour == 9 and da.minute == 0 and times == 1:
            get_city_code('昆明')
            times = 0
        if times == 0 and da.hour ==9 and da.minute == 1:
            times = 1
        time.sleep(1)


if __name__ == '__main__':
    main()
