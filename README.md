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
ffmpeg.exe for windows is required.(ffmpeg is free sofuware)  
PATH of environment variable have to be set for ffmpeg.exe.

## How to build  
BackgroundMaker is built by C++Builder 10.3 (Comminity) in Windows.  
Therefor, Building of BackgroundMaker need C++Builder 10.3 or higher version.

Linux and Mac is not able to build.

### Download of repository

If your environment is to work "git" then
below command is work. 

git clone https://github.com/GouNakano/BackgroundMaker.git  

If not then click "source" button and "downn load zip"

## How to Execute BackgroundMaker

Excute BackgroundMaker.exe by Explorer etc or 
Excute from made shortcut of BackgroundMaker.exe.

## How to Use BackgroundMaker(tutorial)

1. FFmpeg for windows has already set up.
2. Download bin/BackgroundMaker.exe.
3. Download sample/sample(n).mp4
4. Execute BackgroundMaker.exe.

![5](https://user-images.githubusercontent.com/56259253/166137969-0b16ae6e-8185-4aa3-b68e-d931e0f94532.png)

5. Click "Read Movie File" button and then "open" or "開く" button click.   
6. Specify the movie file downloaded in 2 and load it.   

![6](https://user-images.githubusercontent.com/56259253/166138127-23b57a8d-83b8-46b2-b8d4-295a064dcec1.png)

7. Click "Start" button.  

![7](https://user-images.githubusercontent.com/56259253/166138318-1e24e246-2975-48b1-bfe1-8ca6e5b6d838.png)

8. Click "Extract Background" button.  

![8](https://user-images.githubusercontent.com/56259253/166138427-8c2675a6-8c68-423e-99bf-087cb3a24168.png)

9. It is starting to make background image.  

![9](https://user-images.githubusercontent.com/56259253/166138483-1eda9a57-45d3-4f3d-8ce6-35f024c69fb6.png)

10. Click "Save Background Image"
11. Specify file name and then "Save" or "保存" button click.


# License
The source code is licensed MIT. The website content is licensed CC BY 4.0,see LICENSE.


![BackgroundMaker_Window](https://user-images.githubusercontent.com/56259253/165922567-6e286b0f-ca6c-45cd-aac2-f39cdbeec1b8.png)
