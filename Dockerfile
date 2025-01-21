# syntax=docker/dockerfile:1.4
FROM ubuntu:24.04 as build

ARG USERNAME
ARG GROUPNAME
ARG UID
ARG GID

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Tokyo

RUN apt-get update -y && \
    apt-get install -y --assume-yes --no-install-recommends \
    git \
    curl \
    unzip \
    ca-certificates \
    openssh-server \
    sudo \
    locales \
    tzdata \
    language-pack-ja \
    vim \
    less && \
    locale-gen 'en_US.UTF-8' && \
    locale-gen 'ja_JP.UTF-8' && \
    rm /etc/localtime && \
    ln -s /usr/share/zoneinfo/Etc/GMT+9 /etc/localtime && \
    dpkg-reconfigure --frontend noninteractive tzdata && \
    echo "TZ=Asia/Tokyo" >> /etc/environment

RUN curl -L 'https://github.com/neovim/neovim/releases/download/v0.10.3/nvim-linux64.tar.gz' | tar xz -C /usr/local --strip-components=1 && \
    apt-get install -y --assume-yes --no-install-recommends \
    bat \
    zsh \
    tig \
    ripgrep \
    clangd-18 \
    nodejs \
    npm && \
    npm install -g n && \
    n lts

RUN apt-get install -y --assume-yes --no-install-recommends \
    build-essential \
    cmake \
    libgrpc-dev \
    libgrpc++-dev \
    libprotobuf-dev \
    protobuf-compiler-grpc \
    libucx-dev

RUN ssh-keygen -A && \
    mkdir -p /run/sshd && \
    sed -i 's/#PasswordAuthentication yes/PasswordAuthentication yes/g' /etc/ssh/sshd_config

RUN --mount=type=secret,id=PASSWORD export PASSWORD="$(cat /run/secrets/PASSWORD)" && \
    userdel -r ubuntu || true && groupdel -f ubuntu || true && \
    groupadd -g $GID $GROUPNAME && \
    useradd -m -u $UID -g $GID -G sudo $USERNAME && \
    chsh $USERNAME -s /usr/bin/zsh \
    echo "$USERNAME:$PASSWORD" | chpasswd && \
    echo "$USERNAME    ALL=(ALL:ALL) NOPASSWD:ALL" >> /etc/sudoers

USER $USERNAME

RUN --mount=type=ssh,uid=$UID,gid=$GID mkdir -p -m 0700 ~/.ssh && \
    ssh-keyscan -H github.com >> ~/.ssh/known_hosts

RUN git clone https://github.com/skipbit/dots.git /home/$USERNAME/dots && \
    make -C /home/$USERNAME/dots install && \
    git clone https://github.com/junegunn/fzf.git /home/$USERNAME/.fzf && \
    /home/$USERNAME/.fzf/install --all --no-bash --no-fish --key-bindings --completion --no-update-rc

RUN /usr/local/bin/nvim --headless "+Lazy! sync" "+qa" && \
    /usr/local/bin/nvim --headless -c "TSUpdate" "+qa"

EXPOSE 22
CMD ["/usr/sbin/service", "ssh", "start"]

WORKDIR /home/$USERNAME
