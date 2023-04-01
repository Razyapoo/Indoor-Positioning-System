import cv2

cap1 = cv2.VideoCapture(2)
cap2 = cv2.VideoCapture(4)

cap1.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0)
# cap2.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0)

while True:
    _, frame1 = cap1.read()
    _, frame2 = cap2.read()
    cv2.imshow('Camera Feed 1', frame1)
    cv2.imshow('Camera Feed 2', frame2)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap1.release()
cap2.release()
cv2.destroyAllWindows()