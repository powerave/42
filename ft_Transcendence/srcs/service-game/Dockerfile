# service-game/Dockerfile
FROM python:3.12

WORKDIR /app

COPY requirements.txt .
COPY seed_games.py .

RUN pip install --no-cache-dir -r requirements.txt

COPY . .

CMD ["python", "server.py"]
