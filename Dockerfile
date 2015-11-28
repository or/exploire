FROM ubuntu:14.04

COPY exploire /exploire
RUN chmod +x /exploire

ENTRYPOINT ["/exploire"]
