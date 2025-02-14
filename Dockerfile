FROM gocv/opencv:4.10.0

RUN apt-get -y update && apt-get install -y

# Install required dependencies
# <-
RUN apt-get -y install cmake libgflags-dev libatlas-base-dev libeigen3-dev libsuitesparse-dev build-essential g++ gdb autotools-dev libicu-dev libbz2-dev libboost-all-dev libflann-dev libfreeimage3 libfreeimage-dev lz4
RUN apt-get install -y \
    libboost-filesystem-dev \
    libboost-system-dev
# ->

# Configure GCC 9
# <-
RUN apt-get -y install gcc-9 g++-9
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-9
# ->

WORKDIR /
RUN mkdir build_image
WORKDIR /build_image

# Install Glog from source (Ceres)
# (Don't do this anymore)
# <-
# RUN git clone https://github.com/google/glog.git
# WORKDIR /build_image/glog
# RUN cmake -S . -B build -G "Unix Makefiles"
# RUN cmake --build build
# RUN cmake --build build --target install
# ->

######################PYTHON3.12######################

# Download and install Python 3.12 from source
RUN wget https://www.python.org/ftp/python/3.12.0/Python-3.12.0.tgz
RUN tar -xvf Python-3.12.0.tgz
WORKDIR /build_image/Python-3.12.0
RUN ./configure --enable-optimizations
RUN make -j 4
RUN make altinstall

# Clean up Python source files
RUN rm -rf /build_image/Python-3.12.0*

# Update alternatives to use Python 3.12
RUN update-alternatives --install /usr/bin/python3 python3 /usr/local/bin/python3.12 1

# Install pip for Python 3.12
RUN wget https://bootstrap.pypa.io/get-pip.py
RUN python3.12 get-pip.py
RUN rm get-pip.py

# Install Python dependencies using requirements.txt
COPY /Segmentation/requirements.txt /build_image/requirements.txt
RUN python3.12 -m pip install -r /build_image/requirements.txt

# install dependencies for opencv
RUN apt install -y libgl1-mesa-glx libglib2.0-0 \
                       libsm6 libxrender1 libxext6 \
                       libxcb-xinerama0 libxkbcommon-x11-0 \
                      qtbase5-dev qt5-qmake libgl1-mesa-dri \
                      x11-apps

ENV DISPLAY=:0
ENV QT_X11_NO_MITSHM=1


######################PYTHON3.12######################
# Install Glog from package manager
# (Do this instead)
# <-
RUN apt-get -y install libgoogle-glog-dev
# ->

# Install Ceres Solver
# <-
# WORKDIR /build_image
# RUN git clone https://ceres-solver.googlesource.com/ceres-solver
# WORKDIR /build_image/ceres-solver
# RUN mkdir build
# WORKDIR /build_image/ceres-solver/build
# RUN cmake .. -DMINIGLOG=ON -DGFLAGS=OFF -DBUILD_DOCUMENTATION=OFF 
# RUN make -j 4
# RUN make install
# ->
RUN apt-get -y install libceres-dev

# Install code-server
RUN curl -fsSL https://code-server.dev/install.sh | sh

# Set up code-server to run on startup
RUN mkdir -p /home/giokepa/.config/code-server && \
    echo "bind-addr: 0.0.0.0:8080" > /home/giokepa/.config/code-server/config.yaml && \
    echo "auth: password" >> /home/giokepa/.config/code-server/config.yaml

# Run code-server with environment variable for password
# This avoids passing the password directly in CMD or ENTRYPOINT
CMD ["sh", "-c", "code-server --bind-addr 0.0.0.0:8080 /config/workspace"]

