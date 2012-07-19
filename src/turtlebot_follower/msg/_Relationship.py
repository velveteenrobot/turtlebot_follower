"""autogenerated by genmsg_py from Relationship.msg. Do not edit."""
import roslib.message
import struct


class Relationship(roslib.message.Message):
  _md5sum = "186a2f03c193b9a5924bd75d9a9926b7"
  _type = "turtlebot_follower/Relationship"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """string[] prox2
string[] prox4

"""
  __slots__ = ['prox2','prox4']
  _slot_types = ['string[]','string[]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.
    
    The available fields are:
       prox2,prox4
    
    @param args: complete set of field values, in .msg order
    @param kwds: use keyword arguments corresponding to message field names
    to set specific fields. 
    """
    if args or kwds:
      super(Relationship, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.prox2 is None:
        self.prox2 = []
      if self.prox4 is None:
        self.prox4 = []
    else:
      self.prox2 = []
      self.prox4 = []

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    @param buff: buffer
    @type  buff: StringIO
    """
    try:
      length = len(self.prox2)
      buff.write(_struct_I.pack(length))
      for val1 in self.prox2:
        length = len(val1)
        buff.write(struct.pack('<I%ss'%length, length, val1))
      length = len(self.prox4)
      buff.write(_struct_I.pack(length))
      for val1 in self.prox4:
        length = len(val1)
        buff.write(struct.pack('<I%ss'%length, length, val1))
    except struct.error as se: self._check_types(se)
    except TypeError as te: self._check_types(te)

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    @param str: byte array of serialized message
    @type  str: str
    """
    try:
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.prox2 = []
      for i in range(0, length):
        start = end
        end += 4
        (length,) = _struct_I.unpack(str[start:end])
        start = end
        end += length
        val1 = str[start:end]
        self.prox2.append(val1)
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.prox4 = []
      for i in range(0, length):
        start = end
        end += 4
        (length,) = _struct_I.unpack(str[start:end])
        start = end
        end += length
        val1 = str[start:end]
        self.prox4.append(val1)
      return self
    except struct.error as e:
      raise roslib.message.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    @param buff: buffer
    @type  buff: StringIO
    @param numpy: numpy python module
    @type  numpy module
    """
    try:
      length = len(self.prox2)
      buff.write(_struct_I.pack(length))
      for val1 in self.prox2:
        length = len(val1)
        buff.write(struct.pack('<I%ss'%length, length, val1))
      length = len(self.prox4)
      buff.write(_struct_I.pack(length))
      for val1 in self.prox4:
        length = len(val1)
        buff.write(struct.pack('<I%ss'%length, length, val1))
    except struct.error as se: self._check_types(se)
    except TypeError as te: self._check_types(te)

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    @param str: byte array of serialized message
    @type  str: str
    @param numpy: numpy python module
    @type  numpy: module
    """
    try:
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.prox2 = []
      for i in range(0, length):
        start = end
        end += 4
        (length,) = _struct_I.unpack(str[start:end])
        start = end
        end += length
        val1 = str[start:end]
        self.prox2.append(val1)
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.prox4 = []
      for i in range(0, length):
        start = end
        end += 4
        (length,) = _struct_I.unpack(str[start:end])
        start = end
        end += length
        val1 = str[start:end]
        self.prox4.append(val1)
      return self
    except struct.error as e:
      raise roslib.message.DeserializationError(e) #most likely buffer underfill

_struct_I = roslib.message.struct_I