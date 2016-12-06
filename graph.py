

import tensorflow as tf
import numpy as np

with tf.Session() as sess:
    inp = tf.placeholder("float", [None,103], name="inp")
    rew = tf.placeholder("float", [None,1], name="rew")
    w = tf.Variable(tf.random_normal([103,1]), name="w")
    outp = tf.matmul(inp,w,name="outp")


    loss = tf.reduce_mean(tf.square(tf.sub(outp, rew)), name="loss");
    train = tf.train.GradientDescentOptimizer(
    learning_rate=0.1, name="train").minimize(loss)

    sess.run(tf.initialize_all_variables())
    init_all_vars_op = tf.initialize_variables(tf.all_variables(), name='init_all_vars_op')

    
    tf.train.write_graph(sess.graph_def, 'models/', 'graph.pb', as_text=False)


