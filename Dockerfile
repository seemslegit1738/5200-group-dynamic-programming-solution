FROM debian:latest

# Install compiler
RUN apt-get update && apt-get install -y g++ && apt-get clean

WORKDIR /code

COPY . .

# Make script to compile software executable
RUN chmod +x compile.sh

CMD ["./compile.sh"]
