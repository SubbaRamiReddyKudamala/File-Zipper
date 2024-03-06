#include<bits/stdc++.h>
using namespace std;

struct Node
{
    char data;
    unsigned int freq;
    string code;
    Node *left, *right;
    Node()
    {
        left = NULL;
        right = NULL;
    }
};

class huffman
{
    private:
        fstream inFile, outFile;
        string inFileName, outFileName, compFileName;
        Node *root;
        class Compare {
            public:
                bool operator() (Node* l, Node* r)
                {
                    return l->freq > r->freq;
                }
        };
        priority_queue <Node*, vector<Node*>, Compare> minHeap;
        vector <Node*> arr;

    public:

        void compressFiles(string inputFile, string outputFile)
        {
            inFileName = inputFile;
            compFileName = outputFile;
            compress();
        }

        void decompressFiles(string inputFile, string outputFile)
        {
            compFileName = inputFile;
            outFileName = outputFile;
            decompress();
        }

        void createCharArr()
        {
            // creating a character array...........
            for(int i=0;i<128;i++)
            {
                Node *n = new Node();
                n->data = i;
                n->freq = 0;
                arr.push_back(n);
            }
        }
        void createFreaquencyArray()
        {
            // creating a Freaquency Array..............
            char id;
            inFile.open(inFileName, ios::in);

            if (inFile.is_open())
            {
                cout<<"\nfile opened successfully for compressing";
            }
            else cout<<"\nError in opening a file for compressing";

            inFile.get(id);
            while (!inFile.eof()) {
                arr[id]->freq++;
                inFile.get(id);
            } 
            inFile.close(); 
        }
        void createMinHeap()
        {
            // creating a MinHeap..........
            for(int i=0;i<128;i++)
            {
                if(arr[i]->freq>0)
                {
                    minHeap.push(arr[i]);
                }
            }
        }
        void createTree()
        {
            // creating a huffman tree
            priority_queue<Node*,vector<Node*>,Compare>tempPQ = minHeap;
            Node *left, *right;
            while (tempPQ.size() != 1)
            {
                left = tempPQ.top();
                tempPQ.pop();
                    
                right = tempPQ.top();
                tempPQ.pop();
                    
                root = new Node();
                root->freq = left->freq + right->freq;

                root->left = left;
                root->right = right;
                tempPQ.push(root);
            }
        }
        void createCodes(Node* r, string huff)
        {
            // creating a huffman code for the characters
            if (r->left == NULL && r->right == NULL)
            {
                r->code = huff;
                return;
            }

            createCodes(r->left, huff + '0');
            createCodes(r->right, huff + '1');
        }

        int binToDec(string inStr)
        {
            int res = 0;
            for (auto c : inStr) {
                res = res*2+c-'0';
            }
            return res;
        }

        void saveEncodedFile()
        {
            inFile.open(inFileName, ios::in);
            outFile.open(compFileName, ios::out | ios::binary);
            string in = "";
            string s = "";
            char id;
            inFile.get(id);
            while (!inFile.eof()) 
            { 
                s += arr[id]->code;
                while (s.length() > 8) 
                {
                    in += (char)binToDec(s.substr(0, 8));
                    s = s.substr(8);
                }
                inFile.get(id);
            }
            int count = 8 - s.length();
            if (s.length() < 8) 
            {
                s.append(count, '0');
            }
            in += (char)binToDec(s);
            in += (char)count;
            outFile.write(in.c_str(), in.size());
            inFile.close();
            outFile.close();
        }

        void compress()
        {
            createCharArr(); // To create ASCII chars array 0-127
            createFreaquencyArray(); // To find the frequency of each character
            createMinHeap(); // To create minHeap
            createTree(); // To create huffman tree
            createCodes(root,""); // To create huffman code for the characters
            saveEncodedFile(); // To save the encoded file
        }

        string decToBin(int inNum) 
        {
            string temp = "", res = "";
            while (inNum > 0) 
            {
                temp += (inNum % 2 + '0');
                inNum /= 2;
            }
            res.append(8 - temp.length(), '0');
            for (int i=temp.length()-1; i>=0; i--) 
            {
                res += temp[i];
            }
            return res;
        }

        void decompress()
        {
            inFile.open(compFileName, ios::in | ios::binary);
            outFile.open(outFileName, ios::out);

            if (inFile.is_open())
            {
                cout<<"\nfile opened successfully for decompressing";
            }
            else cout<<"\nError in opening a file for decompressing";
            inFile.seekg(-1, ios::end);
            char count0;
            inFile.read(&count0, 1);
            inFile.seekg(0, ios::beg);

            vector<unsigned char> text;
            unsigned char textseg;
            inFile.read(reinterpret_cast<char*>(&textseg), 1);
            while (!inFile.eof()) 
            {
                text.push_back(textseg);
                inFile.read(reinterpret_cast<char*>(&textseg), 1);
            }

            Node *curr = root;
            string path;
            for (int i=0; i<text.size()-1; i++) 
            {
                path = decToBin(text[i]);
                
                if (i == text.size() - 2) 
                {
                    path = path.substr(0, 8 - count0);
                }
                // Traversing huffman tree and writing the resultant data to the file
                for (int j = 0; j < path.size(); j++) 
                {
                    if (path[j] == '0') 
                    {
                        curr = curr->left;
                    }    
                    else 
                    {
                        curr = curr->right;
                    }

                    if (curr->left == NULL && curr->right == NULL) 
                    {
                        char ch=curr->data;
                        outFile.put(ch);
                        curr = root;
                    }
                }
            }
            inFile.close();
            outFile.close();
            
        }
};

int main()
{
    string in,out,comp="";
    huffman h;
    cout<<"Enter the input file name without .txt: ";
    cin>>in;   comp ="compressed_"+in+".huf";
    in +=".txt";
    h.compressFiles(in,comp);
    cout<<"\nFile compressed succesfully.......... to "<<comp;
    cout<<"\n\nEnter the output file name: ";
    cin>>out;
    out +=".txt";
    h.decompressFiles(comp,out);
    cout<<"\nFile decompressed successfully...................\n";
    return 0;
}