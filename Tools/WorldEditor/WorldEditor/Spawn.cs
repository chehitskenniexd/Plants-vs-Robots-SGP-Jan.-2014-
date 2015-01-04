using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Threading.Tasks;

namespace WorldEditor
{
    class Spawn
    {
        string unit;
        int posX, posY;
        string faction;
        Rectangle srcRect;

        public Rectangle SrcRect
        {
            get { return srcRect; }
            set { srcRect = value; }
        }

        public string Faction
        {
            get { return faction; }
            set { faction = value; }
        }

        public int PosX
        {
            get { return posX; }
            set { posX = value; }
        }

        public int PosY
        {
            get { return posY; }
            set { posY = value; }
        }

        public string Unit
        {
            get { return unit; }
            set { unit = value; }
        }

    }
}
