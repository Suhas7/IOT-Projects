from flask import Flask
import subprocess
import os

app = Flask(__name__)

@app.route("/on")
def on():
    os.system("tvservice --preferred && sudo chvt 6 && sudo chvt 7")
    print('on')
    return "on"

@app.route("/off")
def off():
    os.system("tvservice -o")
    print('off')
    return "off"
@app.route("/status")
def status():
    result = subprocess.getoutput("tvservice --status | grep -i 'state 0xa'")
    print("result is: "+result)
    return result

if __name__=="__main__":
    app.run(host="0.0.0.0", port=5000)