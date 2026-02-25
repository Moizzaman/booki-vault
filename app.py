from flask import Flask, render_template, request, jsonify
import subprocess
import os
import threading
import time

app = Flask(__name__)
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
EXE_PATH = os.path.join(BASE_DIR, "password_manager.exe")

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/action', methods=['POST'])
def handle_action():
    cmd = request.form.get('cmd')
    v1 = request.form.get('v1', '')
    v2 = request.form.get('v2', '')
    
    args = [EXE_PATH, cmd]
    if v1: args.append(v1)
    if v2: args.append(v2)
    
    result = subprocess.run(args, capture_output=True, text=True, cwd=BASE_DIR)
    return jsonify({"output": result.stdout.strip()})

@app.route('/shutdown', methods=['POST'])
def shutdown():
    def kill():
        time.sleep(1)
        os._exit(0)
    threading.Thread(target=kill).start()
    return jsonify({"success": True})

if __name__ == '__main__':
    app.run(debug=True)
