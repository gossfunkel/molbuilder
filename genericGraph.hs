type Edges = [Int]

data Node a = Node a Edges deriving (Show)

{- TODO store dijstra step values -}
data Graph a = EmptyGraph | Singleton Node a | Graph [Node a]

`:` :: Graph a -> Graph a -> Graph a
(Graph gs) : (Graph hs) = Graph (gs : hs)

`:` :: Node a -> Graph a -> Graph a
(Node n) : (Graph gs) = Graph (n : gs) 

`:` :: Node a -> Node a -> Graph a
(Node n) : (Node o) = Graph (n : o) 

instance Functor Graph where
	fmap _ EmptyGraph = EmptyGraph
	fmap f (Singleton x) = Singleton (f x)
	fmap f (Graph (Node nData edges):xs) = Graph (Node (f nData) edges): fmap xs)


{- 
TODO map fn with two inputs to each node and each of its neighbours in turn
nmap :: (Node a -> Node a -> Node a) -> Graph a -> Graph a
nmap = 

TODO dijsktra-ordered mapping 
dmap = 
-}
