using System.Collections.Generic;
using System.IO;
using System.Text;
using DDX.Enums;
using DDX.Structs;

namespace DDX.Functions.Bsp
{
    public class BspParsing
    {
        public string MapName;

        public Header Header;
        public List<ushort[]> Edges;
        public Vector3[] Vertices;
        public Face[] OriginalFaces;
        public Face[] Faces;
        public Plane[] Planes;
        public Brush[] Brushes;
        public Brushside[] Brushsides;
        public Node[] Nodes;
        public Leaf[] Leafs;
        public int[] Surfedges;
        public SurfFlag[] TextureInfo;
        public string EntityBuffer;
        //public World world;

        public BspParsing(Stream stream)
        {
            Load(stream);
        }

        public BspParsing(string filePath, string mapname)
        {
            MapName = mapname;

            if (File.Exists(filePath))
            {
                using (var stream = File.OpenRead(filePath))
                {
                    Load(stream);
                }
            }
        }

        #region METHODS - LOAD

        private void Load(Stream stream)
        {
            Header = GetHeader(stream);
            Edges = GetEdges(stream);
            Vertices = GetVertices(stream);
            OriginalFaces = GetOriginalFaces(stream);
            Faces = GetFaces(stream);
            Planes = GetPlanes(stream);
            Surfedges = GetSurfedges(stream);
            TextureInfo = GetTextureInfo(stream);
            Brushes = GetBrushes(stream);
            Brushsides = GetBrushsides(stream);
            EntityBuffer = GetEntities(stream);
            Nodes = GetNodes(stream);
            Leafs = GetLeafs(stream);
        }

        private string GetEntities(Stream stream)
        {
            var lump = Header.Lumps[(int)LumpType.LumpEntities];
            stream.Position = lump.Offset;
            var data = UtilityReader.ReadBytes(stream, lump.Length);
            return Encoding.ASCII.GetString(data);
        }

        private Header GetHeader(Stream stream)
        {
            var header = new Header();
            header.Ident = UtilityReader.ReadInt(stream);

            UtilityReader.BigEndian = header.Ident != 'V' + ('B' << 8) + ('S' << 16) + ('P' << 24);

            header.Version = UtilityReader.ReadInt(stream);
            header.Lumps = new Lump[64];
            for (var i = 0; i < header.Lumps.Length; i++)
            {
                header.Lumps[i] = new Lump();
                header.Lumps[i].Type = (LumpType)i;
                header.Lumps[i].Offset = UtilityReader.ReadInt(stream);
                header.Lumps[i].Length = UtilityReader.ReadInt(stream);
                header.Lumps[i].Version = UtilityReader.ReadInt(stream);
                header.Lumps[i].FourCc = UtilityReader.ReadInt(stream);
            }
            header.MapRevision = UtilityReader.ReadInt(stream);
            return header;
        }

        private List<ushort[]> GetEdges(Stream stream)
        {
            var edges = new List<ushort[]>();
            var lump = Header.Lumps[(int)LumpType.LumpEdges];
            stream.Position = lump.Offset;

            for (var i = 0; i < lump.Length / 2 / 2; i++)
            {
                var edge = new ushort[2];
                edge[0] = UtilityReader.ReadUShort(stream);
                edge[1] = UtilityReader.ReadUShort(stream);
                edges.Add(edge);
            }

            return edges;
        }

        private Vector3[] GetVertices(Stream stream)
        {
            var lump = Header.Lumps[(int)LumpType.LumpVertexes];
            stream.Position = lump.Offset;
            var vertices = new Vector3[lump.Length / 3 / 4];

            for (var i = 0; i < vertices.Length; i++)
            {
                vertices[i] = new Vector3();
                vertices[i].X = UtilityReader.ReadFloat(stream);
                vertices[i].Y = UtilityReader.ReadFloat(stream);
                vertices[i].Z = UtilityReader.ReadFloat(stream);
            }

            return vertices;
        }

        private Face[] GetOriginalFaces(Stream stream)
        {
            var lump = Header.Lumps[(int)LumpType.LumpOriginalfaces];
            stream.Position = lump.Offset;
            var faces = new Face[lump.Length / 56];

            for (var i = 0; i < faces.Length; i++)
            {
                faces[i] = new Face();
                faces[i].PlaneNumber = UtilityReader.ReadUShort(stream);
                faces[i].Side = UtilityReader.ReadByte(stream);
                faces[i].OnNode = UtilityReader.ReadByte(stream);
                faces[i].FirstEdge = UtilityReader.ReadInt(stream);
                faces[i].NumEdges = UtilityReader.ReadShort(stream);
                faces[i].Texinfo = UtilityReader.ReadShort(stream);
                faces[i].Dispinfo = UtilityReader.ReadShort(stream);
                faces[i].SurfaceFogVolumeId = UtilityReader.ReadShort(stream);
                faces[i].Styles = new byte[4];
                faces[i].Styles[0] = UtilityReader.ReadByte(stream);
                faces[i].Styles[1] = UtilityReader.ReadByte(stream);
                faces[i].Styles[2] = UtilityReader.ReadByte(stream);
                faces[i].Styles[3] = UtilityReader.ReadByte(stream);
                faces[i].LightOffset = UtilityReader.ReadInt(stream);
                faces[i].Area = UtilityReader.ReadFloat(stream);
                faces[i].LightmapTextureMinsInLuxels = new int[2];
                faces[i].LightmapTextureMinsInLuxels[0] = UtilityReader.ReadInt(stream);
                faces[i].LightmapTextureMinsInLuxels[1] = UtilityReader.ReadInt(stream);
                faces[i].LightmapTextureSizeInLuxels = new int[2];
                faces[i].LightmapTextureSizeInLuxels[0] = UtilityReader.ReadInt(stream);
                faces[i].LightmapTextureSizeInLuxels[1] = UtilityReader.ReadInt(stream);
                faces[i].OriginalFace = UtilityReader.ReadInt(stream);
                faces[i].NumPrims = UtilityReader.ReadUShort(stream);
                faces[i].FirstPrimId = UtilityReader.ReadUShort(stream);
                faces[i].SmoothingGroups = UtilityReader.ReadUInt(stream);
            }

            return faces;
        }

        private Face[] GetFaces(Stream stream)
        {
            var lump = Header.Lumps[(int)LumpType.LumpFaces];
            stream.Position = lump.Offset;
            var faces = new Face[lump.Length / 56];

            for (var i = 0; i < faces.Length; i++)
            {
                faces[i] = new Face();
                faces[i].PlaneNumber = UtilityReader.ReadUShort(stream);
                faces[i].Side = UtilityReader.ReadByte(stream);
                faces[i].OnNode = UtilityReader.ReadByte(stream);
                faces[i].FirstEdge = UtilityReader.ReadInt(stream);
                faces[i].NumEdges = UtilityReader.ReadShort(stream);
                faces[i].Texinfo = UtilityReader.ReadShort(stream);
                faces[i].Dispinfo = UtilityReader.ReadShort(stream);
                faces[i].SurfaceFogVolumeId = UtilityReader.ReadShort(stream);
                faces[i].Styles = new byte[4];
                faces[i].Styles[0] = UtilityReader.ReadByte(stream);
                faces[i].Styles[1] = UtilityReader.ReadByte(stream);
                faces[i].Styles[2] = UtilityReader.ReadByte(stream);
                faces[i].Styles[3] = UtilityReader.ReadByte(stream);
                faces[i].LightOffset = UtilityReader.ReadInt(stream);
                faces[i].Area = UtilityReader.ReadFloat(stream);
                faces[i].LightmapTextureMinsInLuxels = new int[2];
                faces[i].LightmapTextureMinsInLuxels[0] = UtilityReader.ReadInt(stream);
                faces[i].LightmapTextureMinsInLuxels[1] = UtilityReader.ReadInt(stream);
                faces[i].LightmapTextureSizeInLuxels = new int[2];
                faces[i].LightmapTextureSizeInLuxels[0] = UtilityReader.ReadInt(stream);
                faces[i].LightmapTextureSizeInLuxels[1] = UtilityReader.ReadInt(stream);
                faces[i].OriginalFace = UtilityReader.ReadInt(stream);
                faces[i].NumPrims = UtilityReader.ReadUShort(stream);
                faces[i].FirstPrimId = UtilityReader.ReadUShort(stream);
                faces[i].SmoothingGroups = UtilityReader.ReadUInt(stream);
            }

            return faces;
        }

        private Plane[] GetPlanes(Stream stream)
        {
            var lump = Header.Lumps[(int)LumpType.LumpPlanes];
            var planes = new Plane[lump.Length / 20];
            stream.Position = lump.Offset;

            for (var i = 0; i < planes.Length; i++)
            {
                planes[i] = new Plane();

                var normal = new Vector3();
                normal.X = UtilityReader.ReadFloat(stream);
                normal.Y = UtilityReader.ReadFloat(stream);
                normal.Z = UtilityReader.ReadFloat(stream);

                planes[i].Normal = normal;
                planes[i].Distance = UtilityReader.ReadFloat(stream);
                planes[i].Type = UtilityReader.ReadInt(stream);
            }

            return planes;
        }

        private Brush[] GetBrushes(Stream stream)
        {
            var lump = Header.Lumps[(int)LumpType.LumpBrushes];
            var brushes = new Brush[lump.Length / 12];
            stream.Position = lump.Offset;

            for (var i = 0; i < brushes.Length; i++)
            {
                brushes[i] = new Brush();

                brushes[i].Firstside = UtilityReader.ReadInt(stream);
                brushes[i].Numsides = UtilityReader.ReadInt(stream);
                brushes[i].Contents = (ContentsFlag)UtilityReader.ReadInt(stream);
            }

            return brushes;
        }

        private Brushside[] GetBrushsides(Stream stream)
        {
            var lump = Header.Lumps[(int)LumpType.LumpBrushes];
            var brushsides = new Brushside[lump.Length / 8];
            stream.Position = lump.Offset;

            for (var i = 0; i < brushsides.Length; i++)
            {
                brushsides[i] = new Brushside();

                brushsides[i].Planenum = UtilityReader.ReadUShort(stream);
                brushsides[i].Texinfo = UtilityReader.ReadShort(stream);
                brushsides[i].Dispinfo = UtilityReader.ReadShort(stream);
                brushsides[i].Bevel = UtilityReader.ReadShort(stream);
            }

            return brushsides;
        }

        private int[] GetSurfedges(Stream stream)
        {
            var lump = Header.Lumps[(int)LumpType.LumpSurfedges];
            var surfedges = new int[lump.Length / 4];
            stream.Position = lump.Offset;

            for (var i = 0; i < lump.Length / 4; i++)
            {
                surfedges[i] = UtilityReader.ReadInt(stream);
            }

            return surfedges;
        }

        private SurfFlag[] GetTextureInfo(Stream stream)
        {
            var lump = Header.Lumps[(int)LumpType.LumpTexinfo];
            var textureData = new SurfFlag[lump.Length / 72];
            stream.Position = lump.Offset;

            for (var i = 0; i < textureData.Length; i++)
            {
                stream.Position += 64;
                textureData[i] = (SurfFlag)UtilityReader.ReadInt(stream);
                stream.Position += 4;
            }

            return textureData;
        }

        private Node[] GetNodes(Stream stream)
        {
            var lump = Header.Lumps[(int)LumpType.LumpNodes];
            var nodesData = new Node[lump.Length / 32];
            stream.Position = lump.Offset;

            for (var i = 0; i < nodesData.Length; i++)
            {
                nodesData[i] = new Node();

                nodesData[i].Planenum = UtilityReader.ReadInt(stream);

                nodesData[i].Children = new int[2];
                nodesData[i].Children[0] = UtilityReader.ReadInt(stream);
                nodesData[i].Children[1] = UtilityReader.ReadInt(stream);

                nodesData[i].Mins = new short[3];
                nodesData[i].Mins[0] = UtilityReader.ReadShort(stream);
                nodesData[i].Mins[1] = UtilityReader.ReadShort(stream);
                nodesData[i].Mins[2] = UtilityReader.ReadShort(stream);

                nodesData[i].Maxs = new short[3];
                nodesData[i].Maxs[0] = UtilityReader.ReadShort(stream);
                nodesData[i].Maxs[1] = UtilityReader.ReadShort(stream);
                nodesData[i].Maxs[2] = UtilityReader.ReadShort(stream);

                nodesData[i].Firstface = UtilityReader.ReadUShort(stream);
                nodesData[i].Numfaces = UtilityReader.ReadUShort(stream);
                nodesData[i].Area = UtilityReader.ReadShort(stream);
                nodesData[i].Paddding = UtilityReader.ReadShort(stream);
            }

            return nodesData;
        }

        private Leaf[] GetLeafs(Stream stream)
        {
            var lump = Header.Lumps[(int)LumpType.LumpLeafs];
            var leafData = new Leaf[lump.Length / 56];
            stream.Position = lump.Offset;

            for (var i = 0; i < leafData.Length; i++)
            {
                leafData[i] = new Leaf();

                leafData[i].Contents = (ContentsFlag)UtilityReader.ReadInt(stream);
                leafData[i].Cluster = UtilityReader.ReadShort(stream);
                leafData[i].Area = UtilityReader.ReadShort(stream);
                leafData[i].Flags = UtilityReader.ReadShort(stream);

                leafData[i].Mins = new short[3];
                leafData[i].Mins[0] = UtilityReader.ReadShort(stream);
                leafData[i].Mins[1] = UtilityReader.ReadShort(stream);
                leafData[i].Mins[2] = UtilityReader.ReadShort(stream);

                leafData[i].Maxs = new short[3];
                leafData[i].Maxs[0] = UtilityReader.ReadShort(stream);
                leafData[i].Maxs[1] = UtilityReader.ReadShort(stream);
                leafData[i].Maxs[2] = UtilityReader.ReadShort(stream);

                leafData[i].Firstleafface = UtilityReader.ReadUShort(stream);
                leafData[i].Numleaffaces = UtilityReader.ReadUShort(stream);
                leafData[i].Firstleafbrush = UtilityReader.ReadUShort(stream);
                leafData[i].Numleafbrushes = UtilityReader.ReadUShort(stream);
                leafData[i].LeafWaterDataId = UtilityReader.ReadShort(stream);
            }

            return leafData;
        }

        #endregion

        public Leaf GetLeafForPoint(Vector3 point)
        {
            var node = 0;

            while (node >= 0)
            {
                var pNode = Nodes[node];
                var pPlane = Planes[pNode.Planenum];

                var d = point.DotProduct(pPlane.Normal) - pPlane.Distance;

                node = d > 0 ? pNode.Children[0] : pNode.Children[1];
            }

            return -node - 1 >= 0 && -node - 1 < Leafs.Length
                ? Leafs[-node - 1]
                : new Leaf { Area = -1, Contents = ContentsFlag.ContentsEmpty };
        }

        public bool IsVisible(Vector3 start, Vector3 end)
        {
            var vDirection = end - start;
            var vPoint = start;

            var iStepCount = (int)vDirection.Length();

            vDirection /= iStepCount;

            var pLeaf = new Leaf { Area = -1 };

            while (iStepCount > 0)
            {
                vPoint += vDirection;

                pLeaf = GetLeafForPoint(vPoint);

                if (pLeaf.Area != -1)
                {
                    if ((pLeaf.Contents & ContentsFlag.ContentsSolid) == ContentsFlag.ContentsSolid ||
                        (pLeaf.Contents & ContentsFlag.ContentsDetail) == ContentsFlag.ContentsDetail)
                    {
                        break;
                    }
                }

                iStepCount--;
            }
            return (pLeaf.Contents & ContentsFlag.ContentsSolid) != ContentsFlag.ContentsSolid;
        }
    }
}
