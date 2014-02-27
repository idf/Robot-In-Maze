__author__ = 'mkorpela'
def Override(interface_class):
    """
    provide java-like @Override annotation
    usage: @Override(Interface) as method annotation
    :param interface_class: Class
    :exception: AssertionError
    :return: method
    """

    def overrider(method):
        try:
            assert (method.__name__ in dir(interface_class))
            return method
        except AssertionError:
            print method.__name__+" for "+interface_class.__name__

    return overrider


import warnings
import functools
warnings.simplefilter('always', DeprecationWarning)
def Deprecated(func, msg=None):
    """
    provide java-like @Deprecated annotation
    A decorator which can be used to mark functions
    as deprecated.It will result in a deprecation warning being shown
    when the function is used.
    """

    message = msg or "Use of deprecated function '{}`.".format(func.__name__)

    @functools.wraps(func)
    def wrapper_func(*args, **kwargs):
        warnings.warn(message, DeprecationWarning, stacklevel=2)
        return func(*args, **kwargs)

    return wrapper_func

