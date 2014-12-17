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
        sender.send_string('%s' % i)
    print("data sent")
    for i in range(1, max+1):
        num = int(receiver.recv_string())
        print("Receive %s" % num)
        sum = sum + num
    print("Answer is : %s" % sum)
