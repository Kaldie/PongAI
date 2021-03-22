## Building boost ##
FROM ubuntu:groovy as build_boost

RUN apt update && apt install -y build-essential wget cmake libssl-dev

# We pass the boost version argument as argument
ARG BOOST_MAJOR=1
ARG BOOST_MINOR=75
ARG BOOST_PATCH=0

ENV BOOST_VERSION=${BOOST_MAJOR}.${BOOST_MINOR}.${BOOST_PATCH}
ENV BOOST_VERSION_UNDERSCORE=${BOOST_MAJOR}_${BOOST_MINOR}_${BOOST_PATCH}
ENV BOOST_ROOT=/boost

WORKDIR /

# Get Boost
RUN wget --max-redirect 3 https://dl.bintray.com/boostorg/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_UNDERSCORE}.tar.gz \
    && mkdir -p ${BOOST_ROOT} \
    && tar zxf boost_${BOOST_VERSION_UNDERSCORE}.tar.gz -C ${BOOST_ROOT} --strip-components=1 \
    && cd ${BOOST_ROOT} \
    && ./bootstrap.sh --prefix=/usr/local \
    && ./b2 install \
    && cd / \
    && rm boost_${BOOST_VERSION_UNDERSCORE}.tar.gz \
    && rm -r ${BOOST_ROOT}


## BUILDING Rabbitmq-c ##
FROM build_boost as build_rabbitmq

ENV RABBITMC_C_ROOT=/rabbitmq-c

WORKDIR /

RUN wget https://github.com/alanxz/rabbitmq-c/archive/v0.10.0.tar.gz -O rabbitmq-c.tar.gz \
    && mkdir -p ${RABBITMC_C_ROOT} \
    && tar zxf rabbitmq-c.tar.gz -C ${RABBITMC_C_ROOT} --strip-components=1 \
    && cd ${RABBITMC_C_ROOT} \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. \
    && cmake --build . --target install \
    && cd / \
    && rm rabbitmq-c.tar.gz \
    && rm -r ${RABBITMC_C_ROOT}


## BUILDING SimpleAmqpClient
FROM build_rabbitmq as build_SimpleAmqpClient

ENV SIMPLE_AMQP_CLIENT_ROOT=/SimpleAmqpClient

RUN wget https://github.com/alanxz/SimpleAmqpClient/archive/v2.5.1.tar.gz -O SimpleAmqpClient.tar.gz \
    && mkdir -p ${SIMPLE_AMQP_CLIENT_ROOT} \
    && tar zxf SimpleAmqpClient.tar.gz -C ${SIMPLE_AMQP_CLIENT_ROOT} --strip-components=1 \
    && cd ${SIMPLE_AMQP_CLIENT_ROOT} \
    && mkdir simpleamqpclient-build \
    && cd simpleamqpclient-build \
    && cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. \
    && cmake --build . --target install \
    && rm -r ${SIMPLE_AMQP_CLIENT_ROOT} \
    && ldconfig


FROM build_SimpleAmqpClient

COPY . /usr/src/myapp
WORKDIR /usr/src/myapp

# RUN mkdir build && \
#     cd build && \
#     cmake .. && \
#     cmake --build .