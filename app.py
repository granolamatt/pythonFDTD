from flask import Flask
app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello, World!'
    
app.run(
    debug=False,
    host="127.0.0.1",
    port=9000
)
