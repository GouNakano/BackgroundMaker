# BackgroundMaker
BackgroundMaker is able to make baqckground image file from video file.  
However video is need to make from fixed position camera.

BackgroundMaker is to delete moving objects as forground objects and  
deleted range is filled baqckground image.  
Baqckground image area is where there is no moving area.

Tow pattern example images are raw images and used BackgroundMaker images.

(before1)  
![image0013](https://user-images.githubusercontent.com/56259253/165471273-f7191527-1a99-44cf-a093-5913be7b8cd7.png)

(after1)  
![image](https://user-images.githubusercontent.com/56259253/165472777-fd1a3f77-1903-417b-a700-ed34b7a5688c.png)

(before2 Kyoto-japan)

![kyouto_0132](https://user-images.githubusercontent.com/56259253/165474748-64727f7e-7c52-465e-a2ea-e4a4f11a5951.png)

(after2 Kyoto-japan)

![kyouto](https://user-images.githubusercontent.com/56259253/165475461-c3c32b18-8426-43d5-849c-91a108f8be53.png)

## Operating environment

### OS  
Windows 10 (Windows 8 and 7 seems to be work. However it is not tested.)

(Linux and Mac is not supported)

## Required software  
ffmpeg.exe for windows is required.  
PATH of environment variable have to be set for ffmpeg.exe.

## How to build  
BackgroundMaker is built by C++Builder 10.3 (Comminity) in Windows.  
Therefor, Building of BackgroundMaker need C++Builder 10.3 or higher version.

Linux and Mac is not able to build.

### Download of repository

If your environment is to work "git" then
below command is work. 

git clone https://github.com/GouNakano/BackgroundMaker.git

If not then 
