

import tensorflow as tf
import numpy as np

with tf.Session() as sess:
    inp = tf.placeholder("float", [None,103], name="inp")
    rew = tf.placeholder("float", [None,1], name="rew")
    w1 = tf.Variable(tf.random_normal([103,50]), name="w1")
    w2 = tf.Variable(tf.random_normal([50,15]), name="w2")
    w3 = tf.Variable(tf.random_normal([15,1]), name="w3")
    o1 = tf.matmul(inp,w1)
    o2 = tf.matmul(tf.nn.relu(o1),w2)
    outp = tf.matmul(tf.nn.relu(o2),w3,name="outp")
#    outp = tf.matmul(inp,w,name="outp")


    loss = tf.reduce_mean(tf.square(tf.sub(outp, rew)), name="loss");
    train = tf.train.GradientDescentOptimizer(
    learning_rate=0.0001).minimize(loss, name="train")

    sess.run(tf.initialize_all_variables())
    init_all_vars_op = tf.initialize_variables(tf.all_variables(), name='init_all_vars_op')

    
    tf.train.write_graph(sess.graph_def, 'models/', 'graph.pb', as_text=False)


