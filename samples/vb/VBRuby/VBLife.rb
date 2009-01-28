ActiveScriptRuby.settrace false
require "lifegame"

# VB�Ń��C�t�Q�[���{��
class VBLifeGame
  def initialize(canvas, width=80, height=80, rectsize=6)
    @width = width
    @height = height
    @canvas = canvas
    @lifegame = LifeGame.new(width, height)
    @rectsize = rectsize
    @goflag = false

    # ���C����Window����
    @prevgrid = {}
    @rectangles = {}
  end

  def next
    @lifegame.nextgen
    display
  end

  def initnew
    @lifegame = LifeGame.new(@width, @height)
    @canvas.clear(@width, @height)
  end

  # �\��
  def display
    nextgrid = {}
    @lifegame.each_life {|geom|
      if @prevgrid[geom]
        @prevgrid.delete geom
      else
        @canvas.setrect(geom.x, geom.y)
      end
      nextgrid[geom] = true
    }
    @prevgrid.each_key {|geom|
      @canvas.resetrect(geom.x, geom.y)
    }
    @prevgrid = nextgrid
  end
end

def createlife(canvas, width, height, rectsize)
  g = VBLifeGame.new(canvas, width, height, rectsize)
end
#g = VBLifeGame.new @canvas
