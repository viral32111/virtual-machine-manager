# syntax=docker/dockerfile:1

# docker container run --interactive --tty --pull always --entrypoint bash --rm ghcr.io/viral32111/virtual-machine-manager-development-environment:latest
# docker buildx build --no-cache --pull --file .devcontainer/Dockerfile --tag ghcr.io/viral32111/virtual-machine-manager-development-environment:latest /var/empty

# devcontainer build --no-cache --workspace-folder $PWD

# Start from GCC
FROM gcc:14-bookworm

# Default configuration
ARG USER_ID=1000 \
	USER_NAME=user

# Execute instructions with Bash
SHELL [ "bash", "-c" ]

# Install SSL/TLS support for HTTPs package mirrors
RUN apt-get update && \
	apt-get install --no-install-recommends --yes apt-transport-https ca-certificates && \
	apt-get clean --yes && \
	rm --verbose --recursive /var/lib/apt/lists/*

# Switch to a United Kingdom repository mirror for faster downloads
# NOTE: This is only for x86_64 (amd64), as many mirrors do not serve aarch64 (arm64) packages!
RUN if [[ "${TARGETARCH}" == 'amd64' ]]; then \
		source /etc/os-release && \
		echo "deb https://mirrorservice.org/sites/ftp.debian.org/debian/ ${VERSION_CODENAME} main contrib non-free" | tee /etc/apt/sources.list; \
	fi

# Install locales & internationalisation support
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
	printf '%s\n' \
		'tzdata tzdata/Areas select Europe' \
		'tzdata tzdata/Zones/Europe select London' | debconf-set-selections --verbose && \
	apt-get install --no-install-recommends --yes locales tzdata && \
	printf '%s\n' \
		'C.UTF-8 UTF-8' \
		'en_GB.UTF-8 UTF-8' \
		'en_US.UTF-8 UTF-8' | tee /etc/locale.gen && \
	locale-gen && \
	apt-get clean --yes && \
	rm --verbose --recursive /var/lib/apt/lists/*

# Set the locale - https://www.man7.org/linux/man-pages/man7/locale.7.html
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

# Configure the regular user
ENV USER_ID=${USER_ID} \
	USER_NAME=${USER_NAME} \
	USER_HOME=/home/${USER_NAME} \
	PATH=/home/${USER_NAME}/.local/bin:$PATH \
	HISTFILE=/dev/null \
	EDITOR=/usr/bin/nano \
	PAGER=/usr/bin/less

# Create the regular user
RUN mkdir --verbose --parents ${USER_HOME}/.local/bin && \
	groupadd --gid ${USER_ID} ${USER_NAME} && \
	useradd --home ${USER_HOME} --shell /bin/bash --comment ${USER_NAME} --uid ${USER_ID} --gid ${USER_ID} ${USER_NAME} && \
	chown --changes --recursive ${USER_ID}:${USER_ID} ${USER_HOME}

# Setup privilege escalation
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
		curl wget \
		nano \
		gnupg openssl && \
	apt-get clean --yes && \
	rm --verbose --recursive /var/lib/apt/lists/*

# Install virtualisation tools
RUN apt-get update && \
	apt-get install --no-install-recommends --yes \
		qemu-system-x86 qemu-user qemu-utils \
		swtpm swtpm-tools \
		ovmf && \
	apt-get clean --yes && \
	rm --verbose --recursive /var/lib/apt/lists/*

# Create the read-only empty directory
RUN mkdir --verbose --parents /var/empty && \
	chmod --changes 555 /var/empty

# Switch to the regular user's home directory
# NOTE: Cannot switch to the regular user as Dev Container requires root for updating the UID & GID to match the host's user
WORKDIR ${USER_HOME}

# Colourful prompt - cannot be an ENV instruction as /etc/bash/bashrc overwrites it!
RUN echo 'export PS1="\[\e[1;34m\][\t] \[\e[1;37m\]($?) \[\e[1;32m\]\u@\h\[\e[0m\]:\[\e[1;34m\]\w\[\e[0m\]\$ "' | tee ${USER_HOME}/.bashrc

# Persist Visual Studio Code environments
RUN mkdir --verbose --parents ${USER_HOME}/.vscode-server ${USER_HOME}/.vscode-server-insiders
VOLUME ${USER_HOME}/.vscode-server
VOLUME ${USER_HOME}/.vscode-server-insiders

# Sleep forever so the container never exits - https://containers.dev/guide/dockerfile#docker-compose-image
CMD [ "sleep", "infinity" ]
