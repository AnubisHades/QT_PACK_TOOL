# QT_PACK_TOOL
本工具作用：简化QT CMD打包步骤  
开发环境：QT 5.12.7 32bit
  
## 使用说明  
1.找到QT生成的cmd文件（实际该文件为带环境变量的bat文件快捷方式）  
通过其快捷方式属性可以找到bat文件路径  
![image](https://user-images.githubusercontent.com/5274836/168430796-6b5e024d-4be5-4439-923c-a496dad8d075.png)  
  
2.打开已经编译好的打包工具  
![image](https://user-images.githubusercontent.com/5274836/168430841-a1e68b58-0123-4399-b8ee-70d06dd2df94.png)  
  
3.点击打开QT批处理文件，路径为步骤1中的bat文件，找到文件并确定，该步骤自动提取环境变量路径（如果路径错误会导致打包程序出错）  
![image](https://user-images.githubusercontent.com/5274836/168431385-e771bbe6-fd63-429d-970a-fc7e9a88d570.png)  
![image](https://user-images.githubusercontent.com/5274836/168434060-91633880-dcc3-4b1f-8ddc-5a5d10a77444.png)  
  
4.点击 选择目标文件并选择文件  
![image](https://user-images.githubusercontent.com/5274836/168431578-f48e48c3-721d-40ea-af9e-a8035bd156d3.png)  
![image](https://user-images.githubusercontent.com/5274836/168431726-40d153ad-6d4b-4ce0-bcce-30e1c1768acd.png)  
  
5.点击打包  
![image](https://user-images.githubusercontent.com/5274836/168431862-b7239208-bf3b-4b36-9dea-dd6769bb417c.png)  
打包中请勿关闭  
![image](https://user-images.githubusercontent.com/5274836/168431907-0d2797a0-14bc-4fab-b07c-853212ddb17a.png)  
打包完成，自动在D盘根目录生成D:\0_PkPrg\Res_PackTool_20220514_2259_18  
该路径包含 工具名称，打包时间   
![image](https://user-images.githubusercontent.com/5274836/168431974-aad3abfc-206c-44ab-bb0e-d87581d6a562.png)  
  
6.打包结束，进一步打包请使用其他工具  
![image](https://user-images.githubusercontent.com/5274836/168432263-6b05a5cb-2ec9-466b-bc3d-aeeae96c87f8.png)  

