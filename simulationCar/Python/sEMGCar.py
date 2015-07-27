# -*- coding: utf-8 -*-
"""
Created on Mon Jul 20 11:53:39 2015

@author: eroland
"""

# INTIALISATION
import pygame
import math
import serial
import serial.tools.list_ports
from pygame.locals import *

# This method try to auto detect the Arduino´s serial port
def serial_ports():
    ports = list(serial.tools.list_ports.comports()) #Get a list of available serial ports
    #Looking for Arduino's serial port
    for port in ports:
        if "2341" in port[2]:
            ser = serial.Serial(port[0],9600)
            #print "Arduino in " + port[0]
            return ser
    return None
			
    
############## Main #################################
TURN_SPEED = 3
ACCELERATION = 3
BG= (0,0,0)
MAX_Y = 768
MAX_X = 1024
 
# Serial communication settings
ser = serial_ports()

# initialize the screen with size (MAX_X, MAX_Y)
screen = pygame.display.set_mode((MAX_X, MAX_Y))
pygame.display.set_caption("sEMG Car")
car = pygame.image.load('car2.png')
clock = pygame.time.Clock() # load clock
speed = direction = 0 # start speed & direction
position = (500, 500) # start position
 
play = True
while play:
    k_up = k_left = k_right = 0 # init key values
    clock.tick(30)
    cmd = ser.read()
    # print cmd # Uncomment for debbug
    if cmd == '1':
        k_up = -1 * ACCELERATION
    if cmd == '2':
        k_left = -1 * TURN_SPEED
        k_up = -1 * ACCELERATION
    if cmd == '3':
        k_right = -1 * TURN_SPEED
        k_up = -1 * ACCELERATION
    if cmd == '4':
        k_right = -1 *TURN_SPEED
        k_up = -1 * ACCELERATION      
    screen.fill(BG)
    # SIMULATION
    # .. new speed and direction based on acceleration and turn
    speed = k_up
    direction += (k_right - k_left)
    # .. new position based on current position, speed and direction
    x, y = position
    rad = direction * math.pi / 180
    x += speed*math.sin(rad)
    y += speed*math.cos(rad)
    # make sure the car doesn't exit the screen
    if y < 0:
        y = 0
    elif y > MAX_Y:
        y = MAX_Y
    if x < 0:
        x = 0
    elif x > MAX_X:
        x = MAX_X        
    position = (x, y)
    # RENDERING
    # .. rotate the car image for direction
    rotated = pygame.transform.rotate(car, direction)
    # .. position the car on screen
    rect = rotated.get_rect()
    rect.center = position
    # .. render the car to screen
    screen.blit(rotated, rect)
    pygame.display.flip()
    for event in pygame.event.get():
        if event.type==QUIT:
            play = False
            ser.close()
            
pygame.quit()
sys.exit(0) # quit the game
