import tensorflow as tf

IMG_WIDTH = 256
IMG_HEIGHT = 256


def load(image_file):
    image = tf.io.read_file(image_file)
    image = tf.io.decode_jpeg(image)

    w = tf.shape(image)[1]
    w = w // 2
    input_image = image[:, w:, :]
    real_image = image[:, :w, :]

    input_image = tf.cast(input_image, tf.float32)
    real_image = tf.cast(real_image, tf.float32)

    input_image = tf.image.resize(input_image, [256, 256])
    real_image = tf.image.resize(real_image, [256, 256])

    return real_image, input_image


def load_single(image_file):
    image = tf.io.read_file(image_file)
    image = tf.io.decode_jpeg(image)

    image = tf.cast(image, tf.float32)
    image = resize(image, None, IMG_HEIGHT, IMG_WIDTH)
    image = normalize(image, None)

    return image


def save_result(input_img, output_img, path):
    input_img = tf.image.convert_image_dtype(input_img, tf.uint8)
    output_img = tf.image.convert_image_dtype(output_img, tf.uint8)
    image = tf.concat([input_img, output_img], axis=1)  # side by side

    if image.shape == (256, 512):  # grayscale image
        image = tf.expand_dims(image, axis=-1)
        image = tf.image.grayscale_to_rgb(image)

    image = tf.image.encode_jpeg(image)
    tf.io.write_file(path, image)


def resize(input_image, real_image, height, width):
    input_image = tf.image.resize(
        input_image, [height, width], method=tf.image.ResizeMethod.NEAREST_NEIGHBOR
    )

    if real_image is not None:
        real_image = tf.image.resize(
            real_image, [height, width], method=tf.image.ResizeMethod.NEAREST_NEIGHBOR
        )

        return input_image, real_image

    return input_image


def random_crop(input_image, real_image):
    stacked_image = tf.stack([input_image, real_image], axis=0)
    cropped_image = tf.image.random_crop(
        stacked_image, size=[2, IMG_HEIGHT, IMG_WIDTH, 3]
    )

    return cropped_image[0], cropped_image[1]


# Normalizing the images to [-1, 1]
def normalize(input_image, real_image):
    input_image = (input_image / 127.5) - 1

    if real_image is not None:
        real_image = (real_image / 127.5) - 1
        return input_image, real_image

    return input_image


@tf.function()
def random_jitter(input_image, real_image):
    if tf.random.uniform(()) > 0.5:
        # Random mirroring
        input_image = tf.image.flip_left_right(input_image)
        real_image = tf.image.flip_left_right(real_image)

    return input_image, real_image


def load_image_train(image_file):
    input_image, real_image = load(image_file)
    input_image, real_image = random_jitter(input_image, real_image)
    input_image, real_image = normalize(input_image, real_image)

    return input_image, real_image


def load_image_test(image_file):
    input_image, real_image = load(image_file)
    input_image, real_image = resize(input_image, real_image, IMG_HEIGHT, IMG_WIDTH)
    input_image, real_image = normalize(input_image, real_image)

    return input_image, real_image
