import board
import busio
import digitalio
import paho.mqtt.client as mqtt
import adafruit_rfm9x
import json

RADIO_FREQ_MHZ = 433.0 

CS = digitalio.DigitalInOut(board.CE1)
RESET = digitalio.DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, RADIO_FREQ_MHZ)
rfm9x.node = 255
rfm9x.tx_power = 23

print("Sent Hello World message!")

#health_data = None
#gps_data = None
#alt_data = None

def parse_collated_data(collated_data):
    parsed_data = json.loads(collated_data)

    health = parsed_data["HEALTH"]

    gps = parsed_data["GPS"]

    alt = parsed_data["ALT"]

    return json.dumps(health), json.dumps(gps), json.dumps(alt)
    
def send_radio(data):
    rfm9x.send(bytes(data, "utf-8"))
    print(f"\nSending data through radio: \n{data}")


def formatHeath(data):
    temp = (data.split(" ")[1]).split(",")[0]
    usage = (data.split(" ")[3]).split("}")[0]
    return temp + " " + usage

def formatGps(data):
    data = json.loads(data)
    
    if data['fix'] == 0:
        return "false 0 0 0 0 0"
    returns = "true"

    for attribute, value in data.items():
        if (attribute != 'fix') and (attribute != 'geoidheight') and (attribute != 'fixquality'):
            returns = returns + " " + str(value)

    return returns

def formatAlt(data):
    data = json.loads(data)
    returns = ""
    for attribute, value in data.items():
        returns = returns + " " + str(value)

    return returns.strip()

def check_and_send(health_data, gps_data, alt_data):
    #    global health_data, gps_data, alt_data
   # if health_data is not None and gps_data is not None and alt_data is not None:
    combined_data = "Health:{"+formatHeath(health_data)+ "}\n"+ "GPS:{" +formatGps(gps_data)+ "}\n"+"Altitude:{"+ formatAlt(alt_data) + "}"
    
    #send_radio(combined_data)
      
        #health_data = None
        #gps_data = None
        #alt_data = None

def on_message(client, userdata, message):
    #global health_data, gps_data, alt_data

#    topic = message.topic
    payload = message.payload.decode()
    print(payload)

    health_data, gps_data, alt_data = parse_collated_data(payload)

    check_and_send(health_data, gps_data, alt_data)

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    # Subscribing in on_connect ensures that if we lose the connection and reconnect, subscriptions will be renewed
    client.subscribe("COLLATED")

def setup_mqtt():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect("localhost", 1883, 60)

    client.loop_forever()

if __name__ == "__main__":
    setup_mqtt()
