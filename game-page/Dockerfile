FROM node:23-alpine

WORKDIR /app/game-page

COPY package*.json ./
RUN npm install

COPY . .

CMD ["npm", "run", "build"]
