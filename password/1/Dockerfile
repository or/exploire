FROM ubuntu:14.04

COPY bin/service /service
RUN chmod +x /service
EXPOSE 10000

ENTRYPOINT ["/service"]
