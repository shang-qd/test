import logging
import logging.config


logging.config.fileConfig("logging.config")

logger = logging.getLogger("hyzn")

def setDebug():
    u'设置日志记录方式等级为调试'
    logger.setLevel(logging.DEBUG);

def setInfo():
    u'设置日志记录方式等级为常规信息输出'
    logger.setLevel(logging.INFO);

def debug(str):
    u'用debug方式记录日志'
    logger.debug(str)
    pass

def info(str):
    u'用常规信息方式记录日志'
    logger.info(str)
    pass

def warning(str):
    u'用警告方式记录日志'
    logger.warning(str)
    pass

def error(str):
    u'用错误方式记录日志'
    logger.error(str)
    pass


def critical(str):
    u'用最严重的方式记录日志'
    logger.critical(str)
    pass