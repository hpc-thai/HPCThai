import zmq

context = zmq.Context()

sender = context.socket(zmq.PUSH)
sender.bind("tcp://*:6666")

receiver = context.socket(zmq.PULL)
receiver.bind("tcp://*:7777")

while (True):
    sum = 0
    max = int(input(">>> "))
    for i in range(1, max+1):
        sender.send_string(b'%s' % i)
        print("Sent: %s" % i)
    print("data sent.")
    for i in range(1, max+1):
        num = int(receiver.recv_string())
        print("Received: %s" % num)
        sum = sum + num
    print("\nAnswer is : %s" % sum)
