#SW				= docker
SW				= podman

COMPOSE_FILE	= srcs/docker-compose.yml
ENV_FILE		= srcs/.env
EXAMPLE_ENV		= srcs/.env.example
COMPOSE			= $(SW) compose -f $(COMPOSE_FILE)

#make does the same as make up
all: up

env:
	@if ! [ -f "$(ENV_FILE)" ]; then cp $(EXAMPLE_ENV) $(ENV_FILE); fi

certs-re:
	@printf "Cleaning certs...\n"
	@rm -rf $(filter-out certs/generate.sh certs/mkcert, $(shell find certs -mindepth 1 -maxdepth 1))
	@printf "Creating certs freshly...\n"
	@cd certs && ./generate.sh

clear-tokens:
	@$(COMPOSE) down service-token token-db -v
	@$(COMPOSE) up -d service-token token-db

build: env
	@printf "Building docker network...\n"
	@$(COMPOSE) build

#make up: Builds and starts the network
up: env certs-re
	@printf "Starting docker network...\n"
	@$(COMPOSE) --verbose up --build -d
	@sleep 5 
	@$(MAKE) --no-print-directory seed-games

vite: env
	@printf "Starting stack + frontend-dev profile...\n"
	@printf "Frontend dev (HMR): http://localhost:5173 | API gateway: https://localhost\n"
	@printf "Ensuring frontend dependencies are installed in dev volume...\n"
	@$(COMPOSE) --profile dev stop frontend-dev >/dev/null 2>&1 || true
	@$(COMPOSE) --profile dev run --rm frontend-dev npm install --include=dev
	@$(COMPOSE) --profile dev up -d --remove-orphans

#make down: stop containers
down:
	@printf "Stopping docker network...\n"
	@$(COMPOSE) down

del-volumes: 
	@printf "Stopping docker network and deleting volumes...\n"
	@$(COMPOSE) down -v
	
restart: down up

logs:
	@$(COMPOSE) logs -f --tail=200

#make restart: restarts but doesn not rebuild, volumes persist
restart: down up

#make clean: Stops containers and delete images and volumes
clean:
	@printf "Cleaning up docker network...\n"
	@$(COMPOSE) down -v --remove-orphans
	@printf "Removing frontend dev cache...\n"
	@rm -rf srcs/frontend/.vite
	@printf "Cleaning certs...\n"

fclean:
	@printf "FCleaning up docker network...\n"
	@$(COMPOSE) down -v --rmi all --remove-orphans
	@printf "Removing frontend dev cache...\n"
	@rm -rf srcs/frontend/.vite
	@printf "Cleaning certs...\n"

#make delete: Stops ALL containers on the machine and removes ALL volumes/networks
#(careful if you're running unrelated dockers)
delete: clean
	@containers=$$($(SW) ps -q); \
	if [ -n "$$containers" ]; then \
		$(SW) stop $$containers; \
	fi
	@$(SW) system prune -af

seed-games:
	@echo "Seeding game stats (SQLAlchemy)..."
	docker exec -it game python seed_games.py

re: clean all

.PHONY : all env build up dev down restart logs clean delete re certs-re del-volumes seed-games clear-tokens
