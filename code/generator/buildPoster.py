#!/usr/bin/env python
# -*- coding=utf-8 -*-

# Author  : Sebastien Verel
# Date    : 02/02/2022
# Licence : GNU General Public License as published by the Free Software Foundation, version 3 of the License

import json
import os, sys
import math

#----------------------------------------------------
class Poster:
    ''' 
    @param photoFilename the file name with the description of the photos
    @param posterFileName the file name with the description of the album
    '''
    def __init__(self, posterFileName):
        # read the file with the album description
        with open(posterFileName) as data_file:    
            self.poster = json.load(data_file)

        self.photos = [ None ] * self.poster["size"]
        for p in self.poster["photos"]:
            if p["index"] < self.poster["size"]:
                self.photos[ p["index"] ] = p 

    '''
    Create the header of the page 
    @param numPage the number of the page
    @param photoList the ordered list of photo in the page
    '''
    def create_header_page(self, f):
        f.write("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>\n")
        f.write("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\" version=\"1.1\">\n" % (self.poster["width"], self.poster["height"]))
        f.write(self.poster["header"])
        #f.write("<rect style=\"fill:#6d7d8f;fill-opacity:1;stroke:none;stroke-width:0.776;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1\" width=\"1056\" height=\"202\" x=\"532\" y=\"3\" />")
        #f.write("<text style=\"font-size:80px;line-height:1.25;font-family:sans-serif;-inkscape-font-specification:'sans-serif, Normal'\" x=\"620\" y=\"129\">Quelques vues d'Ecosse</text>\n")

    '''
    Create the bottom of the page 
    @param numPage the number fo the page
    '''
    def create_body_page(self, f, photos_dir, solution):
        for i in range(len(solution.sigma)):
            # position of the photo i on the poster
            position = self.poster["positions"][ solution.sigma[i] ]
            # from center to left top corner
            x = position["x"] - position["width"] / 2
            y = position["y"] - position["height"] / 2

            # dimension of the photo i
            factor = solution.x[i]
            w = position["width"] * factor
            h = position["height"] * factor

            # file name of the photo i
            photo_filename = "%s/%s" % (photos_dir, self.photos[i]["name"])

            # svg code of the corresponding photo
            f.write("<image href=\"%s\" x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\"/>\n"% (photo_filename, x, y, w, h))
        
    '''
    Create the footer of the page 
    @param f output file
    '''
    def create_footer_page(self, f):
        f.write("</svg>\n") # main

    ''' 
    Create all the pages of the album 
    @param html_dir
    @param photos_dir
    @param solution describes the poster
    '''
    def create_poster(self, out_dir, photos_dir, solution, out_file_name):
        f = open(os.path.join(out_dir, "%s" % (out_file_name)), "w")
        self.create_header_page(f)
        self.create_body_page(f, photos_dir, solution)
        self.create_footer_page(f)
        f.close()

#----------------------------------------------------
class Solution:
    ''' 
    @param solutionName
    '''
    def __init__(self, solution_name = ""):
        # quality of the position (to minimize)
        self.fitness = 10^10
        # number of positions
        self.n = 0
        # permutation of the order of the photos: 
        #   sigma[i] is the position of photo i
        #   size of sigma is n
        self.sigma = []
        # relative size of the photo: 
        #   x[i] is the relative size of photo i
        #   size of x is n
        self.x = []

        if (solution_name != ""):
            # read the solution order
            with open(solution_name) as f:
                values = f.readline().split()

                self.fitness = float(values[0])
                self.n       = int(values[1])

                if self.n > 0:
                    self.sigma = [ 0 ] * self.n
                    self.x     = [ 0.0 ] * self.n
                    for i in range(self.n):
                        self.sigma[i] = int(values[2 + i])
                        self.x[i]     = float(values[2 + self.n + i])
                else:
                    self.sigma = []
                    self.x     = []

    def __str__(self):
        s = str(self.fitness)
        
        s += " " + str(self.n)

        for i in range(self.n):
            s += " " + str(self.sigma[i])

        for i in range(self.n):
            s += " " + str(self.x[i])

        return s

#===================================================================
if __name__ == '__main__':
    oi_dir         = "svg"                            # path to svg output files, input photo directory
    poster_name    = "data/poster_48_0.json"          # file name of the poster information
    photos_dir     = "img"                            # path to images from the svg directory (inside oi_dir)
    solution_name  = "data/solution-basic_48.sol"     # file name of the solution which gives the assignement of the photos
    out_file_name  = "poster.svg"                     # name of the output files

    if len(sys.argv) == 6:
        checklist = [2, 4]
        i = 0
        while i < len(checklist) and os.path.exists(sys.argv[ checklist[i] ]):
            i = i + 1

        if i < len(checklist):
            print("File not found: " + sys.argv[ checklist[i] ])
        else:
            oi_dir         = sys.argv[1]
            poster_name    = sys.argv[2]
            photos_dir     = sys.argv[3]
            solution_name  = sys.argv[4]
            out_file_name  = sys.argv[5]

            poster = Poster(poster_name)
            
            solution = Solution(solution_name)

            print(solution)

            poster.create_poster(oi_dir, photos_dir, solution, out_file_name)
    else:
        print("Error: Worng numbver of arguments. Expected 5, actual " + str(len(sys.argv) - 1))
        print("usage: buildPoster.py out_in_directory poster_file_name_json photo_directory solution_file_name out_file_name")
        print("python code/generator/buildPoster.py %s %s %s %s %s" %(oi_dir, poster_name, photos_dir, solution_name, out_file_name))


