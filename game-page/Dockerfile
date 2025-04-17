FROM node:23-slim

WORKDIR /app/game-page

COPY package*.json ./
RUN npm install

COPY . .

CMD ["npm", "run", "build"]
