#ActiveScriptRuby.settrace false
require "lifegame"
require "win32ole"

#$LOAD_PATH.each {|s|print "#{s}\n"}
# VB//cmt{版(2)ライフゲーム本体//}

class VBLifeGame2
  def initialize(width=80, height=80, rectsize=6)
    @width = width
    @height = height
    @rectsize = rectsize
    @lifegame = LifeGame.new(width, height)
    @go = false
    # //cmt{メインのWindow生成//}
    @prevgrid = {}
    # //cmt{canvasの作成//}
    @canvas = WIN32OLE.new("VBLife.Canvas")
    @canvas_event = WIN32OLE_EVENT.new(@canvas, "__Canvas")
    @canvas_event.on_event ("OnNext") { self.next }
    @canvas_event.on_event ("OnNew") { 
      @lifegame = LifeGame.new(@width, @height)
      @canvas.clear(@width, @height)
    }
    @canvas_event.on_event ("OnGo") {
     if @go == false
        @go = true
        while @go == true
          WIN32OLE_EVENT.message_loop #//cmt{Windowsメッセージ処理//}
          self.next
        end
      else
        @go = false
      end
    }
    @canvas_event.on_event ("OnClick") { |x,y| 
       if x.between?(0, @width - 1) && y.between?(0, @height -1)
         geom = Geometry.new(y, x)
         if @lifegame.live?(geom)
           @lifegame.kill(geom)
         else
           @lifegame.born(geom)
         end
         display
       end
    }
    @canvas_event.on_event ("OnQuit") { exit(0) }
  end

  def next
    @lifegame.nextgen
    display
  end

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

  def run
    @canvas.CreateCanvas @width, @height, @rectsize
  end
end

vblife = VBLifeGame2.new(80, 80, 6)
vblife.run

