# Frame_Differ  
帧差法检测 可用于RM比赛的简易雷达站  
这份代码是使用vs编写的 如后续需要使用在rm比赛上 将其转到linux上就好  
由于vs的控制台项目生成的方案太大 因此我没有把.vs文件放进来 如需使用：将代码复制到你的工程包中即可  
帧差法的效果用于检测小物体或重合的物体会有一定的问题（如把两个物体识别到一个物体的情况或过小的物体根本识别不出来）  
但在识别大物体和单独物体是 可以实现很好的跟踪框选效果  
