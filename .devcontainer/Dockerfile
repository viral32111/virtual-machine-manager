# syntax=docker/dockerfile:1

# Start from GCC
FROM gcc:13.2.0-bookworm

# Execute instructions with Bash
SHELL [ "bash", "-c" ]

# Setup timezones & locales
RUN apt-get update && \
	printf '%s\n' \
		'tzdata tzdata/Areas select Europe' \
		'tzdata tzdata/Zones/Europe select London' | debconf-set-selections --verbose && \
	DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends --yes tzdata locales && \
	apt-get clean --yes && \
	rm --verbose --recursive /var/lib/apt/lists/* && \
	printf '%s\n' \
		'C.UTF-8 UTF-8' \
		'en_GB.UTF-8 UTF-8' \
		'en_US.UTF-8 UTF-8' | tee /etc/locale.gen && \
	locale-gen

ENV CHARSET=UTF-8 \
	LANG=en_GB.UTF-8 \
	LANGUAGE=en_GB \
	LC_ALL=en_GB.UTF-8 \
	LC_CTYPE=en_GB.UTF-8 \
	LC_COLLATE=en_GB.UTF-8 \
	LC_ADDRESS=en_GB.UTF-8 \
	LC_MESSAGES=en_GB.UTF-8 \
	LC_MONETARY=en_GB.UTF-8 \
	LC_NUMERIC=en_GB.UTF-8 \
	LC_TIME=en_GB.UTF-8 \
	TZ=Europe/London

# Setup a regular user
ENV USER_ID=1000 \
	USER_NAME=user \
	USER_HOME=/home/user \
	HISTFILE=/dev/null \
	EDITOR=/usr/bin/nano \
	PAGER=/usr/bin/less
RUN mkdir --verbose --parents ${USER_HOME} && \
	groupadd --gid ${USER_ID} ${USER_NAME} && \
	useradd --home ${USER_HOME} --shell /bin/bash --comment ${USER_NAME} --uid ${USER_ID} --gid ${USER_ID} ${USER_NAME} && \
	chown --changes --recursive ${USER_ID}:${USER_ID} ${USER_HOME}

# Setup super-user privileges
RUN apt-get update && \
	apt-get install --no-install-recommends --yes sudo && \
	apt-get clean --yes && \
	rm --verbose --recursive /var/lib/apt/lists/* && \
	usermod --append --groups sudo ${USER_NAME} && \
	printf '%s\n' \
		'%sudo ALL=(ALL:ALL) NOPASSWD: ALL' \
		'%admin ALL=(ALL:ALL) NOPASSWD: ALL' \
		"${USER_NAME} ALL=(ALL:ALL) NOPASSWD: ALL" | tee -a /etc/sudoers

# Install common utilities
RUN apt-get update && \
	apt-get install --no-install-recommends --yes \
		ca-certificates openssl \
		curl wget \
		nano && \
	apt-get clean --yes && \
	rm --verbose --recursive /var/lib/apt/lists/*

# Switch to the regular user
USER ${USER_ID}:${USER_ID}
WORKDIR ${USER_HOME}

# Colourful prompt - cannot be an ENV instruction as /etc/bash/bashrc overwrites it!
RUN echo 'export PS1="\[\e[1;34m\][\t] \[\e[1;37m\]($?) \[\e[1;32m\]\u@\h\[\e[0m\]:\[\e[1;34m\]\w\[\e[0m\]\$ "' | tee ${USER_HOME}/.bashrc

# Persist Visual Studio Code environments
RUN mkdir --verbose --parents ${USER_HOME}/.vscode-server ${USER_HOME}/.vscode-server-insiders
VOLUME ${USER_HOME}/.vscode-server
VOLUME ${USER_HOME}/.vscode-server-insiders

# Sleep forever so container never exits
CMD [ "sleep", "infinity" ]