! include(common.pri)
TEMPLATE = subdirs

SUBDIRS += view \
            model \
            access \
            common \
            test \
            dependency

test.depends = model
logic.depends = access
logic.depends = common
