FROM node:23-alpine

WORKDIR /app/title-page

COPY package*.json ./
RUN npm install

COPY . .

CMD ["npm", "run", "build"]
