FROM node:18-alpine

WORKDIR /app/game-page

COPY package*.json ./
RUN npm install

COPY . .

CMD ["npm", "run", "build"]
