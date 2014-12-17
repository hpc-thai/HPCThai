import zmq

context = zmq.Context()
receiver = context.socket(zmq.PULL)
receiver.connect("tcp://192.168.3.121:6666")

sender = context.socket(zmq.PUSH)
sender.connect("tcp://192.168.3.121:7777")

while (True):
    num = int(receiver.recv_string())
    print("Got: %s" % num)
    num = num * 10
    sender.send_string('%s' % num)    
