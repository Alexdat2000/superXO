FROM golang:1.24-alpine

WORKDIR /middleware
COPY go.* .
RUN go mod download
COPY . .
RUN go build .

ENTRYPOINT ["./middleware"]
