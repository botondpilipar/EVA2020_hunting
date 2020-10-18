! include(common.pri)
TEMPLATE = subdirs

SUBDIRS += view \
            model \
            access \
            common \
            test \
            dependency

view.depends = model
test.depends = model
model.depends = access
