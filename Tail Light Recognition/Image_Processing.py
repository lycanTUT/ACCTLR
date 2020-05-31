import cv2 as cv
import numpy as np

def filter(img, tm):
    kern = (5, 5)
    lower_red = np.array([10, 0, 0])
    upper_red = np.array([160, 255, 255])

    lower_SV = np.array([0, 50, 40])
    upper_SV = np.array([255, 255, 255])

    hsv = cv.cvtColor(cv.blur(img, kern), cv.COLOR_BGR2HSV)
    mask = cv.bitwise_and(~cv.inRange(hsv, lower_red, upper_red),cv.inRange(hsv, lower_SV, upper_SV))
    mask = cv.bitwise_and(mask, tm)
    mask = cv.morphologyEx(mask, cv.MORPH_OPEN, kern)
    mask = cv.morphologyEx(mask, cv.MORPH_CLOSE, kern)
    return mask

def train(vid):
    cap = cv.VideoCapture(vid)
    train_mask = cv.imread("Train/train_mask.jpg", cv.IMREAD_GRAYSCALE)
    frames = 0
    while(cap.isOpened() and frames < 200):
        ret, frame = cap.read()
        frames += 1
        if ret:
            frame = cv.resize(frame, (640, 360), interpolation=cv.INTER_AREA)
            masked = filter(frame, train_mask)

            contours, heir = cv.findContours(masked, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
            contour_list = []
            centers = []
            for contour in contours:
                area = cv.contourArea(contour)
                if area > 150 and len(centers) < 2:
                    contour_list.append(contour)
                    mom = cv.moments(contour)
                    centers.append((int(mom["m10"]/mom["m00"]), int(mom["m01"]/mom["m00"])))

            cv.drawContours(frame, contour_list, -1, (0, 255, 0), 2)
            cv.imshow('Tail Lights', frame)
            cv.imshow('mask', masked)
            if cv.waitKey(25) & 0xFF == ord('q'):
                break
        else:
            break
        print(centers)

    cap.release()
    cv.destroyAllWindows()