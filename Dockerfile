FROM mmore500/conduit

USER root

COPY . /opt/dishtiny

RUN \
  pip3 install -r /opt/dishtiny/docs/requirements.txt \
    && \
  pip3 install -r /opt/dishtiny/third-party/requirements.txt \
    && \
  echo "installed python requirements"

RUN \
  cd /opt/dishtiny/third-party \
    && \
  ./install_dependencies.sh \
    && \
  echo "installed third party dependencies"


RUN \
  TEMP_DEB="$(mktemp)" \
    && \
  wget -O "$TEMP_DEB" 'http://launchpadlibrarian.net/333132059/libhdf5-dev_1.10.0-patch1+docs-4_amd64.deb' \
    && \
  dpkg -i "$TEMP_DEB" \
    && \
  rm -f "$TEMP_DEB" \
    && \
  echo "installed hdf dependencies"

# make sure unprivileged user has access to new files in opt
# adapted from https://stackoverflow.com/a/27703359
# and https://superuser.com/a/235398
RUN \
  chgrp --recursive user /opt/dishtiny \
    && \
  chmod --recursive g+rwx /opt/dishtiny \
    && \
  echo "user granted permissions to /opt/dishtiny"

USER user