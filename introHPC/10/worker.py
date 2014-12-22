import zmq

context = zmq.Context()
receiver = context.socket(zmq.PULL)
receiver.connect("tcp://localhost:6666")

sender = context.socket(zmq.PUSH)
sender.connect("tcp://localhost:7777")

while (True):
    num = int(receiver.recv_string())
    print("Received: %s" % num)
    num = num * 10
    sender.send_string('%s' % num)   
    print("Sent : %s" % num)
