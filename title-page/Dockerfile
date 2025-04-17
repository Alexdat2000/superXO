FROM node:23-alpine AS builder

WORKDIR /title-page
COPY package*.json ./
RUN npm install
COPY . .
RUN npm run build
