FROM centos:7

RUN yum install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
RUN yum install -y centos-release-scl-rh
RUN yum update -y

RUN yum install -y boost169-devel \
                   devtoolset-9-gcc-c++ \
                   make \
                   cmake3

ENTRYPOINT ["./build_unwrapper.sh"]
VOLUME ./cmake