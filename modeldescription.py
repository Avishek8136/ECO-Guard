import tensorflow as tf
model = tf.keras.applications.ResNet50(weights='imagenet')
model.summary()
first_layer = model.layers[0]
input_shape = model.input_shape
output_shape = model.output_shape
model_config = model.get_config()
