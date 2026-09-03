{-# LANGUAGE PatternSynonyms #-}
{-# LANGUAGE TemplateHaskell #-}
module Main where

import Raylib.Core
  ( beginDrawing, clearBackground, closeWindow, endDrawing
  , initWindow, setTargetFPS, getFrameTime, windowShouldClose
  , isKeyReleased
  )
import Raylib.Core.Camera (updateCamera)
import Raylib.Types ( Camera3D (Camera3D), CameraMode (CameraModeFirstPerson)
                    , CameraProjection (CameraPerspective), Color (Color)
                    , KeyboardKey (KeyH), Vector3, pattern Vector3)
--import Raylib.Core.Shapes (drawCircle3D, drawLine3D)
import Raylib.Util (raylibApplication, WindowResources)
import Raylib.Util.Colors (black, blue)

tau :: Float
tau = pi * 2

data Ball = Ball { position :: Vector3
                 , atomicNum :: Int
                 , charge :: Float}

type UserData = (Camera3D, [Ball], WindowResources)

addBall :: Int -> BallBox [Ball] -> BallBox [Ball]
addBall n = let b = Ball (Vector3 10 10 10) n 1 in concat b 

startup :: IO UserData
startup = do
    window <- initWindow 1000 800 "MolBuilder"
    setTargetFPS 60
    let initBall = Ball (Vector3 0 0 0) 1 1
    return (Camera3D (Vector3 0 0 0) (Vector3 2 0 1) (Vector3 0 1 0) 70 CameraPerspective, [initBall], window)

mainLoop :: UserData -> IO UserData
mainLoop (cam, balls, window) = do
    dt <- getFrameTime
    spacePressed <- isKeyReleased KeyH
    if (spacePressed) then let newBalls = addBall 1 balls else let newBalls = balls
    beginDrawing
    clearBackground black
    endDrawing
    return (cam, newBalls, window)

shouldClose :: UserData -> IO Bool
shouldClose _ = windowShouldClose

teardown :: UserData -> IO ()
teardown (_,_,window) = closeWindow $ Just window

raylibApplication 'startup 'mainLoop 'shouldClose 'teardown

