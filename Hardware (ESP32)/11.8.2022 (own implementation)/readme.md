This version establishes communication with a tag and receive data from that tag (not another). Working a little bit worse, because a lot of cases when data are coming only from one anchor. Why?

- With bad wifi signal it is needed more time for communication between server and tag. delay(100). With good wifi signal it is sufficient to use only delay(10). STATE_REQUEST_ACK_FROM_SERVER and STATE_SEND_DISTANCE_TO_SERVER

- TODO: to add anchor balancing technique