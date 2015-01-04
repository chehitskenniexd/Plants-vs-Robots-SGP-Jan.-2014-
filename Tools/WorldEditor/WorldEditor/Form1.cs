using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Serialization;
using System.Xml.Linq;
using System.IO;

namespace WorldEditor
{
    public partial class Form1 : Form
    {
        SGP.CSGP_Direct3D D3D = SGP.CSGP_Direct3D.GetInstance();
        SGP.CSGP_TextureManager TM = SGP.CSGP_TextureManager.GetInstance();

        int tileSet = -1, unitSet = -1, logo = -1, label = -1;

        string imgName;

        bool gridDisplay = true, typeDisplay = true, triggerDisplay = true;
        Tile[,] map = new Tile[10, 10];
        Tile[,] tiles = new Tile[3, 3];
        Spawn[,] spawns = new Spawn[3, 4];
        Spawn[,] spawnMap = new Spawn[10, 10];
        decimal mapW = 10, mapH = 10, tileW = 64, tileH = 64, tilesetW = 3, tilesetH = 3;

        string windDirection = "NONE";

        Point tileSetOffset, mapOffset, selected = new Point(-1, -1), selectedUnit = new Point(-1, -1), mapSelected = new Point(-1, -1);
        int plantCount = 0, robotCount = 0, neutralCount = 0;
        bool initialized = false;

        public Form1()
        {
            InitializeComponent();

            D3D.Initialize(panelTileSet, false);
            D3D.AddRenderTarget(panelMap);
            D3D.AddRenderTarget(panelSelection);
            D3D.AddRenderTarget(panelUnits);
            D3D.AddRenderTarget(panelLogo);

            TM.Initialize(D3D.Device, D3D.Sprite);

            initialized = true;

            //tileSet = TM.LoadTexture("../../../Game/Resources/Images/PvRtemp.bmp");
            label = TM.LoadTexture("../../../Game/Resources/Images/Labels.png");
            unitSet = TM.LoadTexture("../../../Game/Resources/Images/Units.png");
            logo = TM.LoadTexture("../../../Game/Resources/Images/Logo.png");

            if (tileSet >= 0)
                panelTileSet.AutoScrollMinSize = new Size(TM.GetTextureWidth(tileSet), TM.GetTextureHeight(tileSet));

            numericUpDownMapHeight.Value = 10;
            numericUpDownMapWidth.Value = 10;
            numericUpDownTileHeight.Value = 64;
            numericUpDownTileWidth.Value = 64;
            numericUpDownTilesetW.Value = 3;
            numericUpDownTilesetH.Value = 3;
            comboBoxWind.SelectedIndex = 0;
            comboBoxWater.SelectedIndex = 0;

            plantCount = robotCount = neutralCount = 0;

            for (int y = 0; y < mapH; y++)
            {
                for (int x = 0; x < mapW; x++)
                {
                    if (map[y, x].Type == "GRASS" || map[y, x].Type == "SWAMP" || map[y, x].Type == "FOREST")
                        plantCount++;
                    if (map[y, x].Type == "NANO" || map[y, x].Type == "SLUDGE" || map[y, x].Type == "VOLCANO")
                        robotCount++;
                    if (map[y, x].Type == "NEUTRAL" || map[y, x].Type == "WATER" || map[y, x].Type == "MOUNTAIN")
                        neutralCount++;
                }
            }

            string plant = "Plants: " + plantCount.ToString();
            string robot = "Robots: " + robotCount.ToString();
            string neutral = "Neutral: " + neutralCount.ToString();

            listBoxMapStats.Items.Clear();
            listBoxMapStats.Items.Add(plant);
            listBoxMapStats.Items.Add(robot);
            listBoxMapStats.Items.Add(neutral);

            for (int y = 0; y < mapH; y++)
            {
                for (int x = 0; x < mapW; x++)
                {
                    map[y, x].SrcRect = new Rectangle(0, 0, (int)tileW, (int)tileH);
                    map[y, x].Type = "NONE";
                    map[y, x].FlowDirection = "NONE";
                    map[y, x].Trigger = "NONE";
                    map[y, x].LabelRect = new Rectangle(236, 0, 20, 22);
                }
            }

            for (int x = 0; x < tilesetW; x++)
            {
                for (int y = 0; y < tilesetH; y++)
                {
                    tiles[y, x].SrcRect = new Rectangle((int)(x * tileW), (int)(y * tileH), (int)tileW, (int)tileH);
                    tiles[y, x].Type = "NONE";
                    tiles[y, x].FlowDirection = "ALL";
                    tiles[y, x].Trigger = "NONE";
                    tiles[y, x].LabelRect = new Rectangle(236, 0, 20, 22);
                }
            }

            string[] unitType = new String[4];

            unitType[0] = "SCOUT";
            unitType[1] = "TANK";
            unitType[2] = "INFANTRY";
            unitType[3] = "BASE";

            string[] faction = new String[3];

            faction[0] = "PLANT";
            faction[1] = "NEUTRAL";
            faction[2] = "ROBOT";

            for (int x = 0; x < 4; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    spawns[y, x] = new Spawn();
                    spawns[y, x].SrcRect = new Rectangle((int)(x * tileH), (int)(y * tileW), (int)tileH, (int)tileW);
                    spawns[y, x].Unit = unitType[x];
                    spawns[y, x].Faction = faction[y];
                }
            }
        }

        bool looping = true;

        public bool Looping
        {
            get { return looping; }
            set { looping = value; }
        }

        public new void Update()
        {

        }

        public void Render()
        {
            // TileSet 
            D3D.Clear(panelTileSet, Color.White);

            D3D.DeviceBegin();
            D3D.SpriteBegin();

            if (tileSet != -1)
                TM.Draw(tileSet, 2 + tileSetOffset.X, 2 + tileSetOffset.Y);

            if (gridDisplay || typeDisplay)
            {
                for (int x = 0; x < tilesetW; x++)
                {
                    for (int y = 0; y < tilesetH; y++)
                    {
                        if (typeDisplay)
                            TM.Draw(label, (int)(x * tileW + 2 + tileSetOffset.X), (int)(y * tileH + 2 + tileSetOffset.Y), 1.0f, 1.0f, tiles[y, x].LabelRect);

                        if (gridDisplay)
                            D3D.DrawHollowRect(new Rectangle((int)(x * tileW + 2 + tileSetOffset.X), (int)(y * tileH + 2 + tileSetOffset.Y), (int)(tileW), (int)(tileH)), Color.Black, 3);
                    }
                }
            }


            if (selected.X >= 0 && selected.Y >= 0)
                D3D.DrawHollowRect(new Rectangle((int)(selected.X * tileW + 2 + tileSetOffset.X), (int)(selected.Y * tileH + 2 + tileSetOffset.Y), (int)(tileW), (int)(tileH)), Color.Red, 3);

            D3D.SpriteEnd();
            D3D.DeviceEnd();

            D3D.Present();

            if (logo != -1)
            {
                // Logo
                D3D.Clear(panelLogo, Color.White);

                D3D.DeviceBegin();
                D3D.SpriteBegin();

                TM.Draw(logo, 0, 0);

                D3D.SpriteEnd();
                D3D.DeviceEnd();

                D3D.Present();
            }
            // UnitSet
            D3D.Clear(panelUnits, Color.White);

            D3D.DeviceBegin();
            D3D.SpriteBegin();

            if (unitSet != -1)
                TM.Draw(unitSet, 0 + tileSetOffset.X, 0 + tileSetOffset.Y);

            for (int x = 0; x < 4; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    D3D.DrawHollowRect(new Rectangle((int)(x * tileW + 2 + tileSetOffset.X), (int)(y * tileH + 2 + tileSetOffset.Y), (int)(tileW), (int)(tileH)), Color.Black, 3);
                }
            }

            if (selectedUnit.X >= 0 && selectedUnit.Y >= 0)
                D3D.DrawHollowRect(new Rectangle((int)(selectedUnit.X * tileW + 2 + tileSetOffset.X), (int)(selectedUnit.Y * tileH + 2 + tileSetOffset.Y), (int)(tileW), (int)(tileH)), Color.Red, 3);

            D3D.SpriteEnd();
            D3D.DeviceEnd();

            D3D.Present();

            // Selection
            D3D.Clear(panelSelection, Color.White);

            D3D.DeviceBegin();
            D3D.SpriteBegin();

            int tile = (int)(selected.Y * tilesetW + selected.X);

            if (tileSet != -1)
                TM.Draw(tileSet, 1, 1, 1.0f, 1.0f, new Rectangle((int)(selected.X * tileW), (int)(selected.Y * tileH), (int)tileW, (int)tileH));

            D3D.SpriteEnd();
            D3D.DeviceEnd();

            D3D.Present();

            // Map
            D3D.Clear(panelMap, Color.White);

            D3D.DeviceBegin();
            D3D.SpriteBegin();

            for (int y = 0; y < mapH; y++)
            {
                for (int x = 0; x < mapW; x++)
                {
                    if (tileSet != -1)
                        TM.Draw(tileSet, (int)(x * tileW + 1 + mapOffset.X), (int)(y * tileH + 1 + mapOffset.Y), 1.0f, 1.0f, map[y, x].SrcRect);

                    if (unitSet != -1 && spawnMap[y, x] != null)
                        TM.Draw(unitSet, (int)(spawnMap[y, x].PosX * tileW + 1 + mapOffset.X), (int)(spawnMap[y, x].PosY * tileH + 1 + mapOffset.Y), 1.0f, 1.0f, spawnMap[y, x].SrcRect);

                    if (typeDisplay)
                        TM.Draw(label, (int)(x * tileW + 2 + tileSetOffset.X), (int)(y * tileH + 2 + tileSetOffset.Y), 1.0f, 1.0f, map[y, x].LabelRect);
                }
            }

            if (gridDisplay)
            {
                for (int y = 0; y < mapH; y++)
                {
                    for (int x = 0; x < mapW; x++)
                    {
                        D3D.DrawHollowRect(new Rectangle((int)(x * tileW + 1 + mapOffset.X), (int)(y * tileH + 1 + mapOffset.Y), (int)(tileW), (int)(tileH)), Color.Black, 2);
                    }
                }
            }

            if (mapSelected.X >= 0 && mapSelected.Y >= 0)
                D3D.DrawHollowRect(new Rectangle((int)(mapSelected.X * tileW + 2 + mapOffset.X), (int)(mapSelected.Y * tileH + 2 + mapOffset.Y), (int)(tileW), (int)(tileH)), Color.Red, 3);

            D3D.SpriteEnd();
            D3D.DeviceEnd();

            D3D.Present();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            looping = false;
        }

        private void numericUpDownMapWidth_ValueChanged(object sender, EventArgs e)
        {
            decimal oldMapW = mapW, oldMapH = mapH, oldTileSetW = tilesetW, oldTileSetH = tilesetH;
            mapW = numericUpDownMapWidth.Value;
            mapH = numericUpDownMapHeight.Value;
            tileW = numericUpDownTileWidth.Value;
            tileH = numericUpDownTileHeight.Value;
            tilesetW = numericUpDownTilesetW.Value;
            tilesetH = numericUpDownTilesetH.Value;

            if (oldMapH != mapH || oldMapW != mapW)
            {
                panelMap.AutoScrollMinSize = new Size((int)(mapW * tileW), (int)(mapH * tileH));

                Tile[,] tempMap = new Tile[(int)mapH, (int)mapW];
                Spawn[,] tempSpawn = new Spawn[(int)mapH, (int)mapW];

                for (int y = 0; y < mapH; y++)
                {
                    for (int x = 0; x < mapW; x++)
                    {
                        tempMap[y, x].SrcRect = new Rectangle(0, 0, (int)tileW, (int)tileH);
                        tempMap[y, x].Trigger = "NONE";
                        tempMap[y, x].Type = "NEUTRAL";
                        tempMap[y, x].LabelRect = new Rectangle(236, 0, 20, 22);
                    }
                }

                for (int x = 0; x < oldMapW && x < mapW; x++)
                {
                    for (int y = 0; y < oldMapH && y < mapH; y++)
                    {
                        tempMap[y, x] = map[y, x];
                        tempSpawn[y, x] = spawnMap[y, x];
                    }
                }

                spawnMap = tempSpawn;
                map = tempMap;
            }

            if (oldTileSetW != tilesetW || oldTileSetH != tilesetH)
            {
                Tile[,] tempTiles = new Tile[(int)tilesetH, (int)tilesetW];

                for (int x = 0; x < tilesetW; x++)
                {
                    for (int y = 0; y < tilesetH; y++)
                    {
                        tempTiles[y, x].SrcRect = new Rectangle((int)(x * tileW), (int)(y * tileH), (int)tileW, (int)tileH);
                        tempTiles[y, x].Trigger = "NONE";
                        tempTiles[y, x].Type = "NEUTRAL";
                        tempTiles[y, x].LabelRect = new Rectangle(236, 0, 20, 22);
                    }
                }


                for (int x = 0; x < oldTileSetW && x < tilesetW; x++)
                {
                    for (int y = 0; y < oldTileSetH && y < tilesetH; y++)
                    {
                        tempTiles[y, x].Trigger = tiles[y, x].Trigger;
                        tempTiles[y, x].Type = tiles[y, x].Type;
                    }
                }

                tiles = tempTiles;
            }

        }

        private void loadTilesetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();

            ofd.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + "\\..\\..\\..\\Game\\Images");

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                if (tileSet != -1)
                    TM.UnloadTexture(tileSet);

                tileSet = TM.LoadTexture(ofd.FileName);
                imgName = ofd.SafeFileName;
                panelTileSet.AutoScrollMinSize = new Size(TM.GetTextureWidth(tileSet), TM.GetTextureHeight(tileSet));
                numericUpDownTilesetH.Value = TM.GetTextureHeight(tileSet) / tileH;
                numericUpDownTilesetW.Value = TM.GetTextureWidth(tileSet) / tileW;
            }
        }

        private void panelTileSet_Scroll(object sender, ScrollEventArgs e)
        {
            tileSetOffset = new Point(0, 0);

            tileSetOffset.X += panelTileSet.AutoScrollPosition.X;
            tileSetOffset.Y += panelTileSet.AutoScrollPosition.Y;

            Render();
        }

        private void panelMap_Scroll(object sender, ScrollEventArgs e)
        {
            mapOffset = new Point(0, 0);

            mapOffset.X += panelMap.AutoScrollPosition.X;
            mapOffset.Y += panelMap.AutoScrollPosition.Y;

            Render();
        }

        private void panelTileSet_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (tilesetH > 0 && tilesetW > 0)
                {
                    int x = (int)((e.Location.X - tileSetOffset.X) / tileW);
                    int y = (int)((e.Location.Y - tileSetOffset.Y) / tileH);

                    if (x < tilesetW && y < tilesetH)
                    {
                        selected = new Point(x, y);
                        mapSelected = new Point(-1, -1);

                        comboBoxWater.SelectedIndex = comboBoxWater.FindString(tiles[y, x].FlowDirection);
                        comboBoxType.SelectedIndex = comboBoxType.FindStringExact(tiles[y, x].Type);
                        textBox1.Text = tiles[y, x].Trigger;
                    }
                    else
                        selected = new Point(-1, -1);
                }
            }
        }

        private void panelMap_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && (selected.X >= 0 || selectedUnit.X >= 0))
            {
                if (tabControlMain.SelectedIndex == 0)
                {
                    if (tilesetH > 0 && tilesetW > 0)
                    {
                        int x = (int)((e.Location.X - mapOffset.X) / tileW);
                        int y = (int)((e.Location.Y - mapOffset.Y) / tileH);

                        if (x < mapW && x >= 0 && y < mapH && y >= 0)
                        {
                            map[y, x].Type = tiles[selected.Y, selected.X].Type;
                            map[y, x].FlowDirection = tiles[selected.Y, selected.X].FlowDirection;
                            map[y, x].LabelRect = tiles[selected.Y, selected.X].LabelRect;
                            map[y, x].SrcRect = new Rectangle((int)(selected.X * tileW), (int)(selected.Y * tileH), (int)tileW, (int)tileH);
                        }
                    }

                    plantCount = robotCount = neutralCount = 0;

                    for (int y = 0; y < mapH; y++)
                    {
                        for (int x = 0; x < mapW; x++)
                        {
                            if (map[y, x].Type == "GRASS" || map[y, x].Type == "SWAMP" || map[y, x].Type == "FOREST")
                                plantCount++;
                            if (map[y, x].Type == "NANO" || map[y, x].Type == "SLUDGE" || map[y, x].Type == "VOLCANO")
                                robotCount++;
                            if (map[y, x].Type == "NEUTRAL" || map[y, x].Type == "WATER" || map[y, x].Type == "MOUNTAIN")
                                neutralCount++;
                        }
                    }
                    string plant = "Plants: " + plantCount.ToString();
                    string robot = "Robots: " + robotCount.ToString();
                    string neutral = "Neutral: " + neutralCount.ToString();

                    listBoxMapStats.Items.Clear();
                    listBoxMapStats.Items.Add(plant);
                    listBoxMapStats.Items.Add(robot);
                    listBoxMapStats.Items.Add(neutral);
                }
                else
                {
                    int x = (int)((e.Location.X - mapOffset.X) / tileW);
                    int y = (int)((e.Location.Y - mapOffset.Y) / tileH);

                    if (x < mapW && y < mapH && x >= 0 && y >= 0)
                    {
                        Spawn temp = new Spawn();
                        temp.Unit = spawns[selectedUnit.Y, selectedUnit.X].Unit;
                        temp.Faction = spawns[selectedUnit.Y, selectedUnit.X].Faction;
                        temp.SrcRect = spawns[selectedUnit.Y, selectedUnit.X].SrcRect;
                        temp.PosX = x;
                        temp.PosY = y;
                        spawnMap[y, x] = temp;
                    }
                }
            }
            else if (e.Button == MouseButtons.Right && selectedUnit.X >= 0)
            {
                int x = (int)((e.Location.X - mapOffset.X) / tileW);
                int y = (int)((e.Location.Y - mapOffset.Y) / tileH);

                if (x < mapW && y < mapH)
                {
                    spawnMap[y, x] = null;
                }
            }
            else if (e.Button == MouseButtons.Left)
            {
                int x = (int)((e.Location.X - mapOffset.X) / tileW);
                int y = (int)((e.Location.Y - mapOffset.Y) / tileH);

                if (x < mapW && y < mapH)
                {
                    mapSelected = new Point(x, y);

                    if (map[y, x].Type != null)
                        comboBoxType.SelectedIndex = comboBoxType.FindStringExact(map[y, x].Type);

                    if (map[y, x].Trigger != null)
                        textBox1.Text = map[y, x].Trigger;

                    if (map[y, x].FlowDirection != null)
                        comboBoxWater.SelectedIndex = comboBoxWater.FindStringExact(map[y, x].FlowDirection);
                    else
                        comboBoxWater.SelectedIndex = -1;

                }
                else
                    selected = new Point(-1, -1);
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (tileSet != -1)
            {
                for (int x = 0; x < tilesetW; x++)
                {
                    for (int y = 0; y < tilesetH; y++)
                    {
                        if (tiles[y, x].Type == "NONE")
                        {
                            MessageBox.Show("Cannot save map.\nNot all tile types assigned.", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                            return;
                        }
                    }
                }


                for (int y = 0; y < mapH; y++)
                {
                    for (int x = 0; x < mapW; x++)
                    {
                        if (map[y, x].Type == "NONE")
                        {
                            MessageBox.Show("Cannot save map.\nNot all map tiles have assigned type.", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                            return;
                        }
                    }
                }

                SaveFileDialog sfd = new SaveFileDialog();

                sfd.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + "\\..\\..\\..\\Game\\Xmls");

                if (sfd.ShowDialog() == DialogResult.OK)
                {
                    XElement xRoot = new XElement("Sheet");
                    XAttribute xVersion = new XAttribute("version", 0);
                    xRoot.Add(xVersion);

                    XElement xFileName = new XElement("ImageName");
                    XAttribute xName = new XAttribute("Name", imgName);
                    xFileName.Add(xName);
                    xRoot.Add(xFileName);

                    XElement xMap = new XElement("BattleMap");

                    XAttribute xWidth = new XAttribute("MapWidth", mapW);
                    xMap.Add(xWidth);

                    XAttribute xHeight = new XAttribute("MapHeight", mapH);
                    xMap.Add(xHeight);

                    XAttribute xTileWidth = new XAttribute("TileWidth", tileW);
                    xMap.Add(xTileWidth);

                    XAttribute xTileHeight = new XAttribute("TileHeight", tileH);
                    xMap.Add(xTileHeight);

                    XAttribute xWind = new XAttribute("WindDirection", windDirection);
                    xMap.Add(xWind);

                    xRoot.Add(xMap);

                    XElement xTiles = new XElement("Tiles");
                    xMap.Add(xTiles);

                    for (int y = 0; y < mapH; y++)
                    {
                        for (int x = 0; x < mapW; x++)
                        {
                            XElement xTile = new XElement("Tile");

                            XAttribute xTileType = new XAttribute("TileType", map[y, x].Type);
                            xTile.Add(xTileType);

                            XAttribute xTileTrigger = new XAttribute("TileTrigger", map[y, x].Trigger);
                            xTile.Add(xTileTrigger);

                            XAttribute xMapPosX = new XAttribute("MapPosX", x);
                            xTile.Add(xMapPosX);

                            XAttribute xMapPosY = new XAttribute("MapPosY", y);
                            xTile.Add(xMapPosY);

                            XAttribute xTilesetX = new XAttribute("TilesetX", map[y, x].SrcRect.Left);
                            xTile.Add(xTilesetX);

                            XAttribute xTilesetY = new XAttribute("TilesetY", map[y, x].SrcRect.Top);
                            xTile.Add(xTilesetY);

                            if (map[y, x].Type == "WATER" || map[y, x].Type == "SWAMP" || map[y, x].Type == "SLUDGE")
                            {
                                XAttribute xFlow = new XAttribute("WaterFlow", map[y, x].FlowDirection);
                                xTile.Add(xFlow);
                            }

                            xTiles.Add(xTile);
                        }
                    }

                    XElement xSpawns = new XElement("Spawns");
                    XAttribute xUnitImg = new XAttribute("ImageName", "units");
                    xMap.Add(xSpawns);

                    for (int y = 0; y < mapH; y++)
                    {
                        for (int x = 0; x < mapW; x++)
                        {
                            if (spawnMap[y, x] != null)
                            {
                                XElement xSpawn = new XElement("Spawn");

                                XAttribute xType = new XAttribute("UnitType", spawnMap[y, x].Unit);
                                xSpawn.Add(xType);

                                XAttribute xFaction = new XAttribute("Faction", spawnMap[y, x].Faction);
                                xSpawn.Add(xFaction);

                                XAttribute xX = new XAttribute("MapPosX", spawnMap[y, x].PosX);
                                xSpawn.Add(xX);

                                XAttribute xY = new XAttribute("MapPosY", spawnMap[y, x].PosY);
                                xSpawn.Add(xY);

                                XAttribute xLeft = new XAttribute("UnitsetX", spawnMap[y, x].SrcRect.Left);
                                xSpawn.Add(xLeft);

                                XAttribute xTop = new XAttribute("UnitsetY", spawnMap[y, x].SrcRect.Top);
                                xSpawn.Add(xTop);


                                xSpawns.Add(xSpawn);
                            }
                        }
                    }

                    XElement xTileSet = new XElement("TileSet");
                    XAttribute xTilesetW = new XAttribute("Width", tilesetW);
                    xTileSet.Add(xTilesetW);
                    XAttribute xTilesetH = new XAttribute("Height", tilesetH);
                    xTileSet.Add(xTilesetH);
                    xMap.Add(xTileSet);

                    for (int x = 0; x < tilesetW; x++)
                    {
                        for (int y = 0; y < tilesetH; y++)
                        {
                            XElement xTile = new XElement("Tile");

                            XAttribute xType = new XAttribute("TileType", tiles[y, x].Type);
                            xTile.Add(xType);

                            XAttribute xX = new XAttribute("SourceRectLeft", tiles[y, x].SrcRect.Left);
                            xTile.Add(xX);

                            XAttribute xY = new XAttribute("SourceRectTop", tiles[y, x].SrcRect.Top);
                            xTile.Add(xY);

                            XAttribute xLeft = new XAttribute("SourceRectWidth", tiles[y, x].SrcRect.Width);
                            xTile.Add(xLeft);

                            XAttribute xTop = new XAttribute("SourceRectHeight", tiles[y, x].SrcRect.Height);
                            xTile.Add(xTop);

                            xTileSet.Add(xTile);
                        }
                    }

                    xRoot.Save(sfd.FileName);
                }
            }
            else
            {
                MessageBox.Show("Cannot save map.\nNo tileset selected.", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();

            ofd.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + "\\..\\..\\..\\Game\\Xmls");

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                XElement xSheet = XElement.Load(ofd.FileName);

                IEnumerable<XElement> xSections = xSheet.Elements();

                XElement xFilename = xSections.ElementAt(0);
                XAttribute xFile = xFilename.Attribute("Name");
                imgName = Convert.ToString(xFile.Value);

                if (tileSet != -1)
                {
                    TM.UnloadTexture(tileSet);
                    tileSet = -1;
                }

                if (File.Exists("../../../Game/Resources/Images/" + imgName) == false)
                {
                    MessageBox.Show("Cannot load map.\n" + imgName + " not found.", "ERROR", MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation);
                    return;
                }

                tileSet = TM.LoadTexture("../../../Game/Resources/Images/" + imgName);
                panelTileSet.AutoScrollMinSize = new Size(TM.GetTextureWidth(tileSet), TM.GetTextureHeight(tileSet));
                numericUpDownTilesetH.Value = TM.GetTextureHeight(tileSet) / tileH;
                numericUpDownTilesetW.Value = TM.GetTextureWidth(tileSet) / tileW;

                XElement xBattleMap = xSections.ElementAt(1);

                XAttribute xMapWidth = xBattleMap.Attribute("MapWidth");
                int mapWidth = Convert.ToInt32(xMapWidth.Value);

                XAttribute xMapHeight = xBattleMap.Attribute("MapHeight");
                int mapHeight = Convert.ToInt32(xMapHeight.Value);

                XAttribute xTileWidth = xBattleMap.Attribute("TileWidth");
                tileW = Convert.ToInt32(xTileWidth.Value);
                numericUpDownTileWidth.Value = tileW;

                XAttribute xTileHeight = xBattleMap.Attribute("TileHeight");
                tileH = Convert.ToInt32(xTileHeight.Value);
                numericUpDownTileHeight.Value = tileH;

                XAttribute xWind = xBattleMap.Attribute("WindDirection");
                windDirection = Convert.ToString(xWind.Value);
                comboBoxWind.SelectedIndex = comboBoxWind.FindString(windDirection);


                numericUpDownMapWidth.Value = mapWidth;
                numericUpDownMapHeight.Value = mapHeight;

                IEnumerable<XElement> xSubSections = xBattleMap.Elements();
                IEnumerable<XElement> xTiles = xSubSections.ElementAt(0).Elements();
                foreach (XElement xTile in xTiles)
                {
                    XAttribute xTileType = xTile.Attribute("TileType");
                    string Type = Convert.ToString(xTileType.Value);

                    XAttribute xTileTrigger = xTile.Attribute("TileTrigger");
                    string Trigger = Convert.ToString(xTileTrigger.Value);

                    XAttribute xMapPosX = xTile.Attribute("MapPosX");
                    int nLocX = Convert.ToInt32(xMapPosX.Value);

                    XAttribute xMapPosY = xTile.Attribute("MapPosY");
                    int nLocY = Convert.ToInt32(xMapPosY.Value);

                    XAttribute xTilesetX = xTile.Attribute("TilesetX");
                    int nRectLeft = Convert.ToInt32(xTilesetX.Value);

                    XAttribute xTilesetY = xTile.Attribute("TilesetY");
                    int nRectTop = Convert.ToInt32(xTilesetY.Value);

                    map[nLocY, nLocX] = new Tile();
                    map[nLocY, nLocX].Trigger = Trigger;
                    map[nLocY, nLocX].Type = Type;
                    map[nLocY, nLocX].SrcRect = new Rectangle(nRectLeft, nRectTop, (int)tileW, (int)tileH);
                }

                IEnumerable<XElement> xSpawns = xSubSections.ElementAt(1).Elements();
                foreach (XElement xSpawn in xSpawns)
                {
                    XAttribute xType = xSpawn.Attribute("UnitType");
                    string type = Convert.ToString(xType.Value);

                    XAttribute xFaction = xSpawn.Attribute("Faction");
                    string faction = Convert.ToString(xFaction.Value);

                    XAttribute xMapPosX = xSpawn.Attribute("MapPosX");
                    int nLocX = Convert.ToInt32(xMapPosX.Value);

                    XAttribute xMapPosY = xSpawn.Attribute("MapPosY");
                    int nLocY = Convert.ToInt32(xMapPosY.Value);

                    XAttribute xTilesetX = xSpawn.Attribute("UnitsetX");
                    int nRectLeft = Convert.ToInt32(xTilesetX.Value);

                    XAttribute xTilesetY = xSpawn.Attribute("UnitsetY");
                    int nRectTop = Convert.ToInt32(xTilesetY.Value);

                    spawnMap[nLocY, nLocX] = new Spawn();
                    spawnMap[nLocY, nLocX].Unit = type;
                    spawnMap[nLocY, nLocX].Faction = faction;
                    spawnMap[nLocY, nLocX].PosX = nLocX;
                    spawnMap[nLocY, nLocX].PosY = nLocY;
                    spawnMap[nLocY, nLocX].SrcRect = new Rectangle(nRectLeft, nRectTop, (int)tileW, (int)tileH);
                }

                IEnumerable<XElement> xTileset = xSubSections.ElementAt(2).Elements();
                XElement xTilesetElement = xSubSections.ElementAt(2);

                tilesetW = Convert.ToDecimal((xTilesetElement.Attribute("Width")).Value);
                numericUpDownTilesetW.Value = tilesetW;

                tilesetH = Convert.ToDecimal((xTilesetElement.Attribute("Height")).Value);
                numericUpDownTilesetH.Value = tilesetH;

                foreach (XElement xTile in xTileset)
                {
                    XAttribute xType = xTile.Attribute("TileType");
                    string type = Convert.ToString(xType.Value);

                    XAttribute xFaction = xTile.Attribute("SourceRectLeft");
                    int rectL = Convert.ToInt32(xFaction.Value);

                    XAttribute xMapPosX = xTile.Attribute("SourceRectTop");
                    int rectT = Convert.ToInt32(xMapPosX.Value);

                    XAttribute xMapPosY = xTile.Attribute("SourceRectWidth");
                    int rectW = Convert.ToInt32(xMapPosY.Value);

                    XAttribute xTilesetX = xTile.Attribute("SourceRectHeight");
                    int rectH = Convert.ToInt32(xTilesetX.Value);

                    int nLocY = rectT / (int)tileH;
                    int nLocX = rectL / (int)tileW;
                    tiles[nLocY, nLocX] = new Tile();
                    tiles[nLocY, nLocX].Type = type;
                    tiles[nLocY, nLocX].SrcRect = new Rectangle(rectL, rectT, rectW, rectH);
                    int index = comboBoxType.FindStringExact(type);
                    tiles[nLocY, nLocX].LabelRect = new Rectangle(index * 22, 0, 22, 20);
                }

            }

            for (int y = 0; y < mapH; y++)
            {
                for (int x = 0; x < mapW; x++)
                {
                    for (int ty = 0; ty < tilesetH; ty++)
                    {
                        for (int tx = 0; tx < tilesetW; tx++)
                        {
                            if (map[y, x].Type == tiles[ty, tx].Type)
                                map[y, x].LabelRect = tiles[ty, tx].LabelRect;
                        }
                    }
                }
            }
        }

        private void panelUnits_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (tilesetH > 0 && tilesetW > 0)
                {
                    int x = (int)((e.Location.X - tileSetOffset.X) / tileW);
                    int y = (int)((e.Location.Y - tileSetOffset.Y) / tileH);

                    if (x < 4 && y < 3)
                        selectedUnit = new Point(x, y);
                    else
                        selectedUnit = new Point(-1, -1);
                }
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            numericUpDownMapHeight.Value = mapH = 10;
            numericUpDownMapWidth.Value = mapW = 10;

            map = new Tile[10, 10];

            for (int y = 0; y < mapH; y++)
            {
                for (int x = 0; x < mapW; x++)
                {
                    map[y, x].SrcRect = new Rectangle(0, 0, (int)tileW, (int)tileH);
                    map[y, x].Type = "NEUTRAL";
                    map[y, x].Trigger = "NONE";
                    map[y, x].LabelRect = new Rectangle(236, 0, 20, 22);
                }
            }

            spawnMap = new Spawn[10, 10];
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (selected.X != -1)
            {
                tiles[selected.Y, selected.X].Type = comboBoxType.SelectedItem.ToString();
                tiles[selected.Y, selected.X].LabelRect = new Rectangle(comboBoxType.SelectedIndex * 22, 0, 22, 20);
            }
        }

        private void panelMap_Resize(object sender, EventArgs e)
        {
            if (initialized)
            {
                D3D.Resize(panelMap, false);
                D3D.Resize(panelSelection, false);
                D3D.Resize(panelUnits, false);
                D3D.Resize(panelLogo, false);
                D3D.Resize(panelTileSet, false);
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (selected.X != -1 && selected.Y != -1)
            {
                tiles[selected.Y, selected.X].Trigger = textBox1.Text;
            }
            else if (mapSelected.X != -1 && mapSelected.Y != -1)
            {
                map[mapSelected.Y, mapSelected.X].Trigger = textBox1.Text;
            }
        }

        private void toolStripButtonGrid_Click(object sender, EventArgs e)
        {
            gridDisplay = !gridDisplay;
        }

        private void toolStripButtonType_Click(object sender, EventArgs e)
        {
            typeDisplay = !typeDisplay;
        }

        private void toolStripButtonTrigger_Click(object sender, EventArgs e)
        {
            triggerDisplay = !triggerDisplay;
        }

        private void comboBoxWind_SelectedIndexChanged(object sender, EventArgs e)
        {
            windDirection = comboBoxWind.SelectedItem.ToString();
        }

        private void comboBoxWater_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (selected.X != -1)
            {
                if (comboBoxType.SelectedItem != null)
                {
                    tiles[selected.Y, selected.X].Type = comboBoxType.SelectedItem.ToString();
                    tiles[selected.Y, selected.X].LabelRect = new Rectangle(comboBoxType.SelectedIndex * 22, 0, 22, 20);
                }

                if (comboBoxWater.SelectedItem != null)
                    tiles[selected.Y, selected.X].FlowDirection = comboBoxWater.SelectedItem.ToString();
            }
            else if (mapSelected.X != -1 && comboBoxWater.SelectedItem != null)
            {
                map[mapSelected.Y, mapSelected.X].FlowDirection = comboBoxWater.SelectedItem.ToString();
            }
        }
    }
}
