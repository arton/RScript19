# ���W�N���X
class Geometry

  #���W[y,x]�̐���
  def Geometry.[](y,x)
    new(y, x)
  end

  # ������
  def initialize(y, x)
    @y = y
    @x = x
  end
  
  # x, y�̃A�N�Z�T
  attr :y, true
  attr :x, true

  # ���@
  def +(other)
    case other
    when Geometry                                    # other��Geometry��?
      Geometry[@y + other.y, @x + other.x]
    when Array                                       # other��Array��?
      Geometry[@y + other[0], @x + other[1]]
    else
      raise TypeError, 
        "wrong argument type #{other.class} (expected Geometry or Array)"
    end
  end

  # ���@
  def -(other)
    case other
    when Geometry                                    # other��Geometry��?
      Geometry[@y - other.y, @x - other.x]
    when Array                                       # other��Array��?
      Geometry[@y - other[0], @x - other[1]]
    else
      raise TypeError, 
        "wrong argument type #{other.class} (expected Geometry or Array)"
    end
  end

  # ��r
  def ==(other)
    Geometry === other and @x == other.x and @y == other.y
  end

  # �n�b�V���֐�
  def hash
    @x.hash ^ @y.hash
  end

  # �n�b�V����r�֐�
  alias eql? ==

  # ������
  def to_s
    format("%d@%d", @y, @x)
  end

  # �C���X�y�N�g
  def inspect
    format("#<%d@%d>", @y, @x)
  end
end
