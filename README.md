# cherryOS
Toneji Univ. --- Personal Operation System Design

## 如何运行项目

### 安装Bochs

- 安装插件

  ```bash
  sudo apt-get install build-essential
  sudo apt-get install xorg-dev
  sudo apt-get install bison
  sudo apt-get install libgtk2.0-dev
  ```

- 安装Bochs

  - 下载`bochs-2.x.x.tar.gz`包

  - 进入到`bochs-2.x.x.tar.gz`的上级目录

    ```bash
    tar -axvf bochs-2.x.x.tar.gz
    cd bochs-2.x.x.tar.gz
    ./configure --enable-debugger --enable-disasm
    ```

  - 打开Makefile, 找到92行左右的LIBS=..., 在行尾添加-lpthread

    ```bash
    make
    sudo make install
    ```

### 安装NASM

- 下载`nasm-x.x.x.tar.gz`包

- 进入到`nasm-x.x.x.tar.gz`的上级目录

  ```bash
  cd nasm-x.x.x.tar.gz
  ./configure
  make
  sudo make install
  ```

### 修改工作路径

- 进入到cherryOS目录中

- 修改`bochsrc`, 将如下三个路径更换为自己的工作路径

  ![image.png](https://upload-images.jianshu.io/upload_images/12014150-36bc9d6020a34346.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 运行cherryOS

- 在`cherryOS/`中打开命令行

- 输入 `bochs` -> 6 -> c

  ![image.png](https://upload-images.jianshu.io/upload_images/12014150-bd91132886d3fd04.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
