#!/usr/bin/env python3

"""Convert a igrap graphml file to an image"""

__author__ = "Simon Maurer"
__version__ = "0.1.0"
__maintainer__ = "Simon Maurer"
__email__ = "moirelein@gmail.com"
__status__ = "Prototype"

import igraph
import sys, argparse
# import igraph.vendor.texttable
import os

sys.settrace
parser = argparse.ArgumentParser(description='This script allows to convert a igraph graphml file to an image')
# parser.add_argument( '-p', '--plot', action='store_true', help='plot the graph of the folded system' )
parser.add_argument( '-c', dest='curved', action='store_true', help='used curved edges' )
parser.add_argument( '-f', metavar="FORMAT", dest='format', choices=['graphml', 'gml'], default='graphml', help='set the format of the input graph (default: graphml)' )
parser.add_argument( '-l', metavar="LAYOUT", dest='layout', choices=['circle', 'drl', 'fr', 'kk', 'lgl', 'random'], default='kk', help='set the layout of the plotted graph (default: kk)' )
parser.add_argument( '-o', metavar="OUTFILE", dest='output', default='out', help='set the output path of the result (default: out.[FORMAT])' )
parser.add_argument( '-x', metavar="PIXEL", dest='x', default=1000, type=int, help='define the image with (default: 1000)' )
parser.add_argument( '-y', metavar="PIXEL", dest='y', default=1000, type=int, help='define the image height (default: 1000)' )
parser.add_argument( '-s', metavar="PIXEL", dest='s', default=40, type=int, help='define the node size (default: 40)' )
parser.add_argument( '-S', metavar="STANDARD", dest='S', choices=['qHD', 'HD', 'HD+', 'FHD', 'QHD', 'QHD+', '4K', '5K', '8K'], help='A High-definition TV standard resolution' )
parser.add_argument( '-I', metavar="IGNORE", dest='ignore', type=int, nargs='+', help='A list of vertext IDs to ignore' )
parser.add_argument( '-E', metavar="iEXCLUSIVE", dest='exclusive', type=int, nargs='+', help='A list of exclusive vertex IDs to draw (all others are ignored)' )
parser.add_argument( 'net', metavar="NET", nargs=1, help='the dependency graph of the PNSC' )
# parser.add_argument( 'outfile', nargs=1, metavar="OUTFILE", help="the graph file to be written" )
args = parser.parse_args()

def main():
    """main program entry point"""
    g = igraph.load( args.net[0], format=args.format )

    if args.ignore and len(args.ignore) > 0:
        g.delete_vertices(args.ignore)
    if args.exclusive and len(args.exclusive) > 0:
        g.delete_vertices([x for x in [*range(0, g.vcount())] if x not in args.exclusive])

    outfile = os.path.splitext(os.path.basename(args.net[0]))
    g.vs()['size'] = args.s;
    g.vs()['color'] = 'gray';
    g.es['curved'] = args.curved;
    for v in g.vs:
        v['label'] = v['id'] + "\n" + v['label']
        if(v['func'] != "smx_rn"):
            v['label'] += "\n" + ('!' if v['location'] else '') + v['func'];
            v['shape'] = 'rect';
        if( v['tt'] > 0 ):
            v['shape'] = 'diamond';
    for e in g.es:
        if( e['len'] > 1 ):
            e['label'] += "[" + str(int(e['len'])) + "]"
        if( e['dsrc'] ):
            e['color'] = '#ff0000';
        if( e['ddst'] ):
            e['color'] = '#0000ff';
        if( e['dsrc'] and e['ddst'] ):
            e['color'] = '#ff00ff';

    resolution = args.S

    if( resolution == 'qHD' ):
        x = 960;
        y = 540;
    elif( resolution == 'HD' ):
        x = 1280;
        y = 720;
    elif( resolution == 'HD+' ):
        x = 1600;
        y = 900;
    elif( resolution == 'FHD' ):
        x = 1920;
        y = 1080;
    elif( resolution == 'QHD' ):
        x = 2560;
        y = 1440;
    elif( resolution == 'QHD+' ):
        x = 3200;
        y = 1800;
    elif( resolution == '4K' ):
        x = 3840;
        y = 2160;
    elif( resolution == '5K' ):
        x = 5120;
        y = 2880;
    elif( resolution == '8K' ):
        x = 7680;
        y = 4320;
    else:
        x = args.x;
        y = args.y;
    box = args.s/2 + 10;
    out = igraph.Plot( target=outfile[0] + ".svg", bbox=( 0, 0, x, y ), background="white" )
    out.add( g, bbox=( box, box, x-box, y-box ), layout=g.layout(args.layout), autocurve=True )
    out.save()

if __name__ == "__main__":
    main()
