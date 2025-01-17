FROM --platform=linux/amd64 ubuntu:22.04 AS build

# Update apps on the base image
RUN apt-get -y update && apt-get install -y

# Install the Clang compiler
RUN apt-get -y install clang

RUN apt-get -y install cmake

RUN apt-get -y install git

RUN apt-get -y update && apt-get install -y

WORKDIR /usr/app

COPY . .

RUN chmod +x ./setup.sh

RUN ./setup.sh

FROM --platform=linux/amd64 ubuntu:22.04 AS runtime

WORKDIR /usr/app

COPY --from=build /usr/app ./

CMD [ "./build/cpplox" ]
